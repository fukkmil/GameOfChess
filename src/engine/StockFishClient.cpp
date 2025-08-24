#include "StockFishClient.h"
#include <QThread>
#include <QtGlobal>

StockfishClient::StockfishClient(QObject *parent)
    : QObject(parent) {
    connect(&m_proc, &QProcess::readyReadStandardOutput,
            this, &StockfishClient::onReadyRead);
    connect(&m_proc, &QProcess::errorOccurred,
            this, &StockfishClient::onError);
    connect(&m_proc, &QProcess::started,
            this, &StockfishClient::onStarted);

    m_proc.setProcessChannelMode(QProcess::MergedChannels);
}

StockfishClient::~StockfishClient() {
    quit();
}

void StockfishClient::start(const QString &enginePath) {
    if (m_proc.state() != QProcess::NotRunning) {
        quit();
    }
    m_uciOk = false;
    m_readyOk = false;
    m_options.clear();

    m_proc.start(enginePath);
    if (!m_proc.waitForStarted(1000)) {
        emit errorText(QString("Не удалось запустить движок: %1").arg(enginePath));
        return;
    }
    requestUciHandshake();
}

void StockfishClient::quit() {
    if (m_proc.state() != QProcess::NotRunning) {
        m_quitting_ = true;
        send("quit");
        m_proc.waitForFinished(200);
        if (m_proc.state() != QProcess::NotRunning) {
            m_proc.kill();
            m_proc.waitForFinished(200);
        }
        m_quitting_ = false;
    }
}

bool StockfishClient::isRunning() const {
    return m_proc.state() != QProcess::NotRunning;
}

void StockfishClient::newGame() {
    send("ucinewgame");
    isReady();
}

void StockfishClient::isReady() {
    m_readyOk = false;
    send("isready");
}

void StockfishClient::setOption(const QString &name, const QString &value) {
    if (!m_uciOk) return;
    if (!m_options.isEmpty() && !m_options.contains(name)) {
        return;
    }
    send(QString("setoption name %1 value %2").arg(name, value));
}

void StockfishClient::setSkillLevel(int skill0to20) {
    setOption("Skill Level", QString::number(qBound(0, skill0to20, 20)));
}

void StockfishClient::setDifficultyElo(int elo, bool limitStrength) {
    setOption("UCI_LimitStrength", limitStrength ? "true" : "false");
    setOption("UCI_Elo", QString::number(elo));
}

void StockfishClient::setPositionFEN(const QString &fen, const QStringList &uciMoves) {
    QString cmd = QString("position fen %1").arg(fen);
    if (!uciMoves.isEmpty()) {
        cmd += " moves " + uciMoves.join(' ');
    }
    send(cmd);
}

void StockfishClient::setPositionFromStartpos(const QStringList &uciMoves) {
    QString cmd = "position startpos";
    if (!uciMoves.isEmpty()) {
        cmd += " moves " + uciMoves.join(' ');
    }
    send(cmd);
}

void StockfishClient::goDepth(int depth) {
    send(QString("go depth %1").arg(depth));
}

void StockfishClient::goMovetime(int ms) {
    send(QString("go movetime %1").arg(ms));
}

void StockfishClient::goClock(int wtimeMs, int btimeMs, int wincMs, int bincMs) {
    send(QString("go wtime %1 btime %2 winc %3 binc %4")
        .arg(wtimeMs).arg(btimeMs).arg(wincMs).arg(bincMs));
}

void StockfishClient::send(const QString &line) {
    if (m_proc.state() == QProcess::NotRunning) return;
    QByteArray bytes = (line + "\n").toUtf8();
    m_proc.write(bytes);
}

void StockfishClient::requestUciHandshake() {
    send("uci");
}

void StockfishClient::onStarted() {
    requestUciHandshake();
}

void StockfishClient::onReadyRead() {
    while (m_proc.canReadLine()) {
        const QByteArray raw = m_proc.readLine();
        const QString s = QString::fromUtf8(raw).trimmed();

        if (s.isEmpty()) continue;

        if (s.startsWith("id name ")) {
            const QString name = s.mid(8);
            emit engineBanner(name, QString());
            continue;
        }
        if (s.startsWith("id author ")) {
            const QString author = s.mid(10);
            emit engineBanner(QString(), author);
            continue;
        }
        if (s.startsWith("option name ")) {
            const QString name = s.mid(QString("option name ").size()).section(' ', 0, 0);
            m_options.insert(name);
            continue;
        }
        if (s == "uciok") {
            m_uciOk = true;
            if (m_options.contains("Threads"))
                setOption(
                    "Threads", QString::number(qMax(1, QThread::idealThreadCount())));
            if (m_options.contains("Hash")) setOption("Hash", "128");
            isReady();
            continue;
        }
        if (s == "readyok") {
            m_readyOk = true;
            emit engineReady();
            continue;
        }
        if (s.startsWith("bestmove ")) {
            const QString rest = s.mid(QString("bestmove ").size());
            const QString best = rest.section(' ', 0, 0);
            QString ponder;
            if (rest.contains(" ponder ")) {
                ponder = rest.section(" ponder ", 1, 1).trimmed();
            }
            emit bestMove(best, ponder);
            continue;
        }
        if (s.startsWith("info ")) {
            emit info(s);
            continue;
        }

        emit info(s);
    }
}

void StockfishClient::onError(QProcess::ProcessError e) {
    if (m_quitting_) return;
    QString msg = "Неизвестная ошибка процесса.";
    switch (e) {
        case QProcess::FailedToStart: msg = "Движок не стартовал (неправильный путь или нет прав).";
            break;
        case QProcess::Crashed: msg = "Движок упал.";
            break;
        case QProcess::Timedout: msg = "Таймаут операции процесса.";
            break;
        case QProcess::WriteError: msg = "Ошибка записи в процесс.";
            break;
        case QProcess::ReadError: msg = "Ошибка чтения из процесса.";
            break;
        case QProcess::UnknownError: default: break;
    }
    emit errorText(msg);
}
