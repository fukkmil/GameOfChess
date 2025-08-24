#ifndef STOCKFISHCLIENT_H
#define STOCKFISHCLIENT_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QRegularExpression>
#include <QTimer>
#include <QSet>

class StockfishClient : public QObject {
    Q_OBJECT
public:
    explicit StockfishClient(QObject* parent = nullptr);
    ~StockfishClient() override;

    void start(const QString& enginePath);
    void quit();
    bool isRunning() const;

    void newGame();
    void isReady();

    void setOption(const QString& name, const QString& value);
    void setSkillLevel(int skill0to20);
    void setDifficultyElo(int elo, bool limitStrength = true);

    void setPositionFEN(const QString& fen, const QStringList& uciMoves = {});
    void setPositionFromStartpos(const QStringList& uciMoves);

    void goDepth(int depth);
    void goMovetime(int ms);
    void goClock(int wtimeMs, int btimeMs, int wincMs = 0, int bincMs = 0);

signals:
    void engineReady();
    void bestMove(const QString& uciMove, const QString& ponder);
    void info(const QString& line);
    void errorText(const QString& message);
    void engineBanner(const QString& name, const QString& author);

private slots:
    void onReadyRead();
    void onError(QProcess::ProcessError);
    void onStarted();

private:
    void send(const QString& line);
    void requestUciHandshake();

    QProcess m_proc;
    bool m_uciOk = false;
    bool m_readyOk = false;
    bool m_quitting_ = false;
    QSet<QString> m_options;
};
#endif // STOCKFISHCLIENT_H