#include <QCoreApplication>
#include <stdlib.h>
#include <clocale>
#include "logic/robot.h"
#include "logic/airrobot.h"
#include "shared/configreader.h"
#include "shared/config.h"
#include "shared/logger.h"
#include "testPRM/testprmalgorithm.h"
#include "testPRM/testastarprm.h"
#include "testPRM/testgraph.h"
#include "testPRM/testevaluationprm.h"

#ifdef TEST
#include "test/pathplanning/RRT/testrrtnode.h"
#include "test/pathplanning/RRT/testrrtnodecomparator.h"
#include "test/pathplanning/RRT/testrrtalgorithm.h"
#include "test/coordination/testauction.h"
#include "test/coordination/testfrontierauction.h"
#include "test/coordination/testvictimauction.h"
#include "test/coordination/testcoordinationmodule.h"
//#include "test/coordination/testintersect.h"
#include "test/exploration/testevaluationfunctions.h"
#include "shared/utilities.h"
#include <QDebug>
#include "test/exploration/testweightmatrix.h"
#include "test/exploration/testweightreader.h"


#endif

struct config_t {
    QString configFile;
    QString usarAddress; quint16 usarPort;
    QString upisAddress; quint16 upisPort;
    QString wssAddress; quint16 wssPort;
    bool usarAddressSet, usarPortSet;
    bool upisAddressSet, upisPortSet;
    bool wssAddressSet, wssPortSet;
    QString location, rotation;
    QString bsLocation;
    int id;
    int nrobots;
    bool nrobotsSet;
    QString robotType;
};

struct config_t initConf(){
    struct config_t conf = {
        "poaret.conf",
        "", 0, "", 0, "", 0,
        false, false, false, false, false, false,
        "0,0,1", "0,0,0", "0,0,0",
        0, 4, false, "p3at"
    };
    return conf;
}

struct config_t parseArgs(const QStringList &args) {
    struct config_t conf = {
        "poaret.conf",
        "", 0, "", 0, "", 0,
        false, false, false, false, false, false,
        "0,0,0", "0,0,0", "0,0,0",
        0, 4, false, "p3at"
    };

    for(int i = 0; i < args.length(); i++) {
        const QString &arg = args[i];

        if(arg == "--usar-port" || arg == "-p") {
            conf.usarPort = args[++i].toInt();
            conf.usarPortSet = true;
        } else if(arg == "--usar-addr" || arg == "-a") {
            conf.usarAddress = args[++i];
            conf.usarAddressSet = true;
        } else if(arg == "--upis-port" || arg == "-o") {
            conf.upisPort = args[++i].toInt();
            conf.upisPortSet = true;
        } else if(arg == "--upis-addr" || arg == "-d") {
            conf.upisAddress = args[++i];
            conf.upisAddressSet = true;
        } else if(arg == "--wss-port" || arg == "-w") {
            conf.wssPort = args[++i].toInt();
            conf.wssPortSet = true;
        } else if(arg == "--wss-addr" || arg == "-s") {
            conf.wssAddress = args[++i];
            conf.wssAddressSet = true;
        } else if(arg == "--location" || arg == "-l") {
            conf.location = args[++i];
        } else if(arg == "--rotation" || arg == "-r") {
            conf.rotation = args[++i];
        } else if(arg == "--bs-location" || arg == "-b") {
            conf.bsLocation = args[++i];
        } else if(arg == "--id" || arg == "-i") {
            conf.id = args[++i].toInt();
        } else if(arg == "--n-robots" || arg == "-n") {
            conf.nrobots = args[++i].toInt();
            conf.nrobotsSet = true;
        } else if(arg == "--config" || arg == "-c"){
            conf.configFile = args[++i];
        } else if(arg == "--type" || arg =="-t"){
            conf.robotType = args[++i];
        }
    }

    return conf;
}

#define PREFIX
//#include "test/coordination/testintersect.h"
#include "test/pathplanning/RRT/testrrtnode.h"
int main(int argc, char *argv[])
{
    ::setlocale(LC_ALL, "en");
    //putenv("LANG=en");

    QCoreApplication a(argc, argv);
    struct config_t conf = initConf();

    QLocale::setDefault(QLocale::English);

    // Load Configuration
    Config::ConfigReader reader(conf.configFile);
    reader.readFileAndCompileConfigs();

    conf = parseArgs(a.arguments());

    // Override configuration with command line arguments
    if(conf.usarAddressSet) Config::usarAddress = conf.usarAddress;
    if(conf.upisAddressSet) Config::upisAddress = conf.upisAddress;
    if(conf.wssAddressSet)  Config::wssAddress  = conf.wssAddress;
    if(conf.usarPortSet)    Config::usarPort    = conf.usarPort;
    if(conf.upisPortSet)    Config::upisPort    = conf.upisPort;
    if(conf.wssPortSet)     Config::wssPort     = conf.wssPort;
    if(conf.nrobotsSet)     Config::robotCount  = conf.nrobots;

    QStringList pose = conf.bsLocation.split(",");
    double x = pose[0].toDouble(), y = pose[1].toDouble();
    Config::baseStationPose = Data::Pose(x, y, 0);

    logger.addSTDOUTOutput();
    logger.setOutputFormat(Logger::PrependCustom);
    logger.setCustomOutputFormat("%T | ");
    logger.setOutputFormat(Logger::PrependNothing);

    int mapping = logger.addFileOutput(QString(PREFIX "poaretSlam_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*slam.*"), mapping);

    int mapping2 = logger.addFileOutput(QString(PREFIX "poaretFastSlam_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*fastslam2\\.cpp"), mapping2);

    logger.addFileOutput(QString(PREFIX "poaret_all_%1.log").arg(conf.id), false);

    int coordination = logger.addFileOutput(QString(PREFIX "poaretCoordination_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*(coordination).*"), coordination);

    int frontier = logger.addFileOutput(QString(PREFIX "poaretFrontier_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*(frontier).*"), frontier);

    int victimAllocation = logger.addFileOutput(QString(PREFIX "poaretVictimAllocation_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*(victim).*"), victimAllocation);

    int exploration = logger.addFileOutput(QString(PREFIX "poaretExploration_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*(exploration).*"), exploration);

    int logic = logger.addFileOutput(QString(PREFIX "poaretLogic_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*(logic).*"), logic);

    int pathPlanner = logger.addFileOutput(QString(PREFIX "poaretPathPlanner_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*(pathPlanner).*"), pathPlanner);

    int victim = logger.addFileOutput(QString(PREFIX "poaretVictim_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*(victimDetection).*"), victim);

    int hybridAStar = logger.addFileOutput(QString(PREFIX "poaretHybridAStar_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*ybrid.*"), hybridAStar);

    int sonar = logger.addFileOutput(QString(PREFIX "poaretSonar_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*middleware.*"), sonar);

    int robotController = logger.addFileOutput(QString(PREFIX "poaretController_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*robotcontroller.*"), robotController);

    int rutting = logger.addFileOutput(QString("poaretRouting_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*connection.*"), rutting);

    int messaging = logger.addFileOutput(QString(PREFIX "poaretMessaging_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*wsscontroller.*"), messaging);

    //PRM
    int prm = logger.addFileOutput(QString(PREFIX "prm_%1.log").arg(conf.id), false);
    logger.setFilenameFilter(QRegExp(".*prm.*"), prm);
    //

    Config::robotID = conf.id;

    if(conf.robotType == "p3at")
    {
        //PRM
#ifdef __TESTPRM__
        TestPRM::TestPRMAlgorithm test;
        TestPRM::TestGraph test2;
        TestPRM::TestAStarPRM test3;
        TestPRM::TestEvaluationPRM test4;
        //        char* argv2[]={"Poaret.exe","-v1",NULL};
        char* argv2[]={"Poaret.exe",NULL};
        int argc2=sizeof(argv2)/sizeof(char*)-1;
        QTest::qExec(&test,argc2,argv2);
        QTest::qExec(&test2,argc2,argv2);
        QTest::qExec(&test3,argc2,argv2);
        QTest::qExec(&test4,argc2,argv2);
        //
#else
        //        qDebug() << "*******************Creo robot p3at*****************************************";
        Robot piRobot(conf.location, conf.rotation, conf.id, false);
        Config::robotMaxSpeed = 0.9;
        Config::robotType = 0;

        piRobot.spawnRobot();
        //        qDebug() << "*******************Avvia applicazione*****************************************";
        return a.exec();
#endif
    }
    if(conf.robotType == "air")
    {
        AirRobot airRobot(conf.location, conf.rotation, conf.id);
        Config::robotMaxSpeed = 2;
        Config::robotType = 2;
        airRobot.spawnRobot();
        qDebug() << "w il PoAReT";
        return a.exec();
    }
    if(conf.robotType == "kenaf")
    {
        Robot kenafRobot(conf.location, conf.rotation, conf.id, true);
        qDebug() << "w il PoAReT";
        Config::robotMaxSpeed = 1.69;
        Config::robotType = 1;
        kenafRobot.spawnRobot();
        return a.exec();
    }
}
