#ifndef ANALYZE_H
#define ANALYZE_H

#include <QtCore>
#include "network.h"

class Client;
class TrainerTeam;
class Player;
class BattleChoice;
class TeamBattle;
class BattleConfiguration;

/* Commands to dialog with the server */
namespace NetworkCli
{
    enum Command
    {
	WhatAreYou = 0,
	WhoAreYou,
	Login,
	Logout,
	SendMessage,
	PlayersList,
	SendTeam,
	ChallengeStuff,
	EngageBattle,
	BattleFinished,
	BattleMessage,
	BattleChat,
        KeepAlive,
        AskForPass,
        Register,
        PlayerKick,
        PlayerBan
    };

    enum ProtocolError
    {
	UnknownCommand = 0
    };
}

/* Analyzes the messages received from the network and emits the corresponding signals.
   Also allows you to send your own messages to the network

    The client actually uses this widgets to send orders and receive commands from the outside. */

class Analyzer : public QObject
{
    Q_OBJECT
public:
    Analyzer();

    /* functions called by the client */
    void login(const TrainerTeam &team);
    void sendMessage(const QString &message);
    void connectTo(const QString &host, quint16 port);
    void sendTeam(const TrainerTeam & team);
    void sendChallengeStuff(quint8 desc, int id);
    void sendBattleResult(int result);
    bool isConnected() const;

    /* Convenience functions to avoid writing a new one every time */
    void notify(int command);
    template<class T>
    void notify(int command, const T& param);
    template<class T1, class T2>
    void notify(int command, const T1& param1, const T2& param2);
    template<class T1, class T2, class T3>
    void notify(int command, const T1& param1, const T2& param2, const T3 &param3);
signals:
    /* to send to the network */
    void sendCommand(const QByteArray &command);
    /* to send to the client */
    void connectionError(int errorNum, const QString &errorDesc);
    void protocolError(int errorNum, const QString &errorDesc);
    void connected();
    void disconnected();
    void messageReceived(const QString &mess);
    /* player from the players list */
    void playerReceived(const Player &p);
    /* login of a player */
    void playerLogin(const Player &p);
    void playerLogout(int id);
    /* challengerelated */
    void challengeStuff(int desc, int id);
    void battleStarted(int id, const TeamBattle &myteam, const BattleConfiguration &conf);
    void battleFinished(int res);
    void battleMessage(const QByteArray &mess);
    void passRequired(const QString &salt);
    void notRegistered(bool);
    void playerKicked(int p, int src);
    void playerBanned(int p, int src);

public slots:
    /* slots called by the network */
    void error();
    void commandReceived (const QByteArray &command);

    /* by the battle window */
    void battleCommand(const BattleChoice &comm);
    void battleMessage(const QString &mess);

private:
    /* The connection to the outside */
    Network &socket();

    Network mysocket;
};

template<class T>
void Analyzer::notify(int command, const T& param)
{
    QByteArray tosend;
    QDataStream out(&tosend, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << uchar(command) << param;

    emit sendCommand(tosend);
}

template<class T1, class T2>
void Analyzer::notify(int command, const T1& param1, const T2 &param2)
{
    QByteArray tosend;
    QDataStream out(&tosend, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << uchar(command) << param1 << param2;

    emit sendCommand(tosend);
}

template<class T1, class T2, class T3>
void Analyzer::notify(int command, const T1& param1, const T2 &param2, const T3 &param3)
{
    QByteArray tosend;
    QDataStream out(&tosend, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << uchar(command) << param1 << param2 << param3;

    emit sendCommand(tosend);
}

#endif // ANALYZE_H
