#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include "../worldmodel/worldmodel.h"
#include "beam_save.h"

#include<iostream>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<string>
#include<algorithm>
#include<vector>
#include<climits>
#include<float.h>
#include<string.h>
#include<ctime>
#include <boost/cstdint.hpp>

typedef uint64_t uint64;

extern int agentBodyType;

struct playerType {
    int id;
    bool isEnemy = false;
    double dist_to_ball = 100;
    int onball_age = 0;
    int offball_age = 0;
    double time = 0;
}teammate_msg[12], opponent_msg[12];


/*
 * Real game beaming.
 * Filling params x y angle
 * 开场阵型
 */
void NaoBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {
    //示例，有需要请自行修改
    if(worldModel->getSide()==SIDE_LEFT){

        if(worldModel->getUNum()==1){
            beamX = -14.8;
            beamY = 0;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==11){
            beamX = -2.3;
            beamY = 0;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==2){
            beamX = -12.3;
            beamY = -7.2;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==3){
            beamX = -12.3;
            beamY = -2.2;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==4){
            beamX = -12.3;
            beamY = 6.3;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==5){
            beamX = -9.6;
            beamY = -4.2;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==6){
            beamX = -9.6;
            beamY = 0;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==7){
            beamX = -9.6;
            beamY = 4.2;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==8){
            beamX = -5;
            beamY = 0;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==9){
            beamX = -2.6;
            beamY = -8.2;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==10){
            beamX = -2.6;
            beamY = 8.2;
            beamAngle = 0;
        }
    }
    if(worldModel->getSide()==SIDE_RIGHT){

        if(worldModel->getUNum()==1){
            beamX = -14.8;
            beamY = 0;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==11){
            beamX = -2.3;
            beamY = 0;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==2){
            beamX = -13.3;
            beamY = -6.2;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==3){
            beamX = -13.3;
            beamY = 1.3;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==4){
            beamX = -13.3;
            beamY = 4.3;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==5){
            beamX = -2.6;
            beamY = -8.3;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==6){
            beamX = -4.6;
            beamY = -6.7;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==7){
            beamX = -6.6;
            beamY = -3.2;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==8){
            beamX = -6.6;
            beamY = 3.2;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==9){
            beamX = -4.6;
            beamY = 6.7;
            beamAngle = 0;
        }
        else if(worldModel->getUNum()==10){
            beamX = -2.6;
            beamY = 8.2;
            beamAngle = 0;
        }
    }
}


SkillType NaoBehavior::selectSkill()
{
    //示例，有需要请自行修改
    worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    worldModel->getRVSender()->clearStaticDrawings();
    int Nearest2ball = findClosestPlayer2Ball();
    double distance,angle;
    getTargetDistanceAndAngle(ball,distance,angle);
    //画出球的位置
    //worldModel->getRVSender()->drawPoint("ball",ball.getX(),ball.getY(),20.0f, RVSender::PINK);

    //开球模式
    if(worldModel->getPlayMode() == PM_KICK_OFF_LEFT || worldModel->getPlayMode() == PM_KICK_OFF_RIGHT)
    {
        return kickOff();
    }

    //PLAY_ON mode正常对局
    else
    {
	//角色分配
        if(Nearest2ball == worldModel->getUNum() && worldModel->getUNum() > 1)
        {
            return onball();	//持球者角色
        }
        //守门员
        else if(worldModel->getUNum() == 1)
        {
            isPKGoal = false;
            if(worldModel->getPlayMode() == PM_CORNER_KICK_LEFT || worldModel->getPlayMode() == PM_CORNER_KICK_RIGHT)
            {
                return goToTargetRelative(worldModel->g2l(VecPosition(-14.5,0,0)),angle);
            }
            else
            {
                return NaoBehavior::selectKeeperSkills(ball,distance,angle);
            }
        }
        //other players
        else
        {
//            VecPosition onballPos = worldModel->getTeammate(teammate_msg[0].id);
//            int mynum = worldModel->getUNum();
//            if(mynum >= 2 && mynum <= 4){
//                //后卫
//                return SKILL_STAND;
//            }
//            else if(mynum >= 5 && mynum <= 11){
//                //前锋
////                VecPosition target = worldModel->getBall();
//                if(opponent_msg[0].dist_to_ball>2){
//                    //球周围没有对方球员 //2m开外
//                    //在onball周围三个位置环行
//                    VecPosition targets[3];
//                    for(int i = 0; i < 3; i++){
//                        targets[i]=onballPos+onballPos.getVecPositionFromPolar(2.5,30+i*60,0);
//                        cout<<"target["<<i<<"]"<<targets[i]<<endl;
//                        if(me.getDistanceTo(targets[i])<0.5){
//                            return goToTarget(targets[i]);
//                        }
//                    }
//                    //其余人向x方向跑
//                    return goToTarget(VecPosition(me.getX()+5,me.getY()));
//                }
//                VecPosition target = collisionAvoidance(true,true,false,0.5,0.5,ball,true);
//                if(me.getDistanceTo(ball)>1.5){
//                    return goToTarget(ball);
//                }

//                else{
//                    return SKILL_STAND;
//                }
//            }

            if(ball.getX()>=0)
                current_beam = demoMode_1(ball);
            else
                current_beam = demoMode_2(ball);
            last_beam.clear();  //delete the data before
            for(int i = WO_TEAMMATE2; i < WO_TEAMMATE1+NUM_AGENTS; ++i)
            {
                if(i != Nearest2ball)
                {
                    last_beam.push_back(i);         //确定要走阵型的机器人球员的Unum
                    if(last_beam.size() == 9)
                    {
                        break;
                    }
                }
            }
            for(int i=0;i<(int)last_beam.size();i++)
            {
                if(last_beam[i] == worldModel->getUNum())
                {
                    VecPosition target = collisionAvoidance(true,true,true,1,0.5,current_beam[i],true);   //假设Unum为i，则把阵型点集合中第i个坐标点设为相应机器人的目标点
                   
                    if(me.getDistanceTo(target) > 0.25)
                    {
                        // Far away from the ball so walk toward target offset from the ball
                        return goToTarget(target);
                    }
                    //面向球
                    double rot,dis;
                    getTargetDistanceAndAngle(ball,dis,rot);    //面向球
                    return goToTargetRelative(target,rot);
                }
            }
        }
        //return goToTarget(ball);
    }
}

//开球站位
SkillType NaoBehavior::kickOff()
{
    //我方开球
    if((worldModel->getSide() == SIDE_LEFT && worldModel->getPlayMode() == PM_KICK_OFF_LEFT) || (worldModel->getSide() == SIDE_RIGHT && worldModel->getPlayMode() == PM_KICK_OFF_RIGHT))
    {
	/***************************************
	*******我方开球总体的站位或走位***********
	**************参赛者自己编写*************
	****************************************/
    if(worldModel->getUNum() == findClosestPlayer2Ball()){
        if(me.getDistanceTo(ball)>1){
            return goToTarget(VecPosition(0,0.2,0));
        }
//        if(me.getDistanceTo(ball)<1&&me.getTheta()<3){
//            return
//        }
        return kickBall(KICK_FORWARD,VecPosition(15,6,0));
    }
    else
        return SKILL_STAND;	//示例，站着不动，有需要请自行修改
    }
    //对方开球
    else if((worldModel->getSide() == SIDE_LEFT && worldModel->getPlayMode() == PM_KICK_OFF_RIGHT) || (worldModel->getSide() == SIDE_RIGHT && worldModel->getPlayMode() == PM_KICK_OFF_LEFT))
    {
	/***************************************
	*****对方开球时我方的总体的站位或走位*****
	**************参赛者自己编写*************
	****************************************/
    return SKILL_STAND;	//示例，站着不动，有需要请自行修改
    }
    return SKILL_STAND;
}


//找出离球最近的球员当持球者
int NaoBehavior::findClosestPlayer2Ball()
{
    	/***************************************
	************参赛者自己编写***************
	****************************************/
//    double dist[11];
    if(worldModel->getTime()==teammate_msg[0].time){
        return teammate_msg[0].id;
    }

    for(int i = 1; i <= 11; i++){
        //更新己方位置
        teammate_msg[i].id=i;
        teammate_msg[i].time=worldModel->getTime();
        VecPosition mypos = worldModel->getTeammate(i);
        if(mypos!=VecPosition(0,0,0))
            teammate_msg[i].dist_to_ball = mypos.getDistanceTo(ball);
        else{
            teammate_msg[i].dist_to_ball = me.getDistanceTo(ball);
        }
        //更新对方位置
        opponent_msg[i].id=i;
        opponent_msg[i].time=worldModel->getTime();
        VecPosition youpos = worldModel->getTeammate(i+11);
        cout<<"num "<<i<<"  opp "<<youpos<<endl;
//        if(mypos!=VecPosition(0,0,0))
            opponent_msg[i].dist_to_ball = youpos.getDistanceTo(ball);
//        cout << "num: " << i+1 << "  dist: "<<dist[i]<<endl;
//        cout << "ball x: " << ball.getX() << "y: " << ball.getY()<<endl;
//        cout << "num: " << i+1 <<" x: " << worldModel->getTeammate(i+1).getX() << " y: " << worldModel->getTeammate(i+1).getY()<<endl;
//        cout << "num: " << i+1 <<" x: " << worldModel->getTeammate(i+1).getX() << " y: " << worldModel->getTeammate(i+1).getY()<<endl;

    }

//    double min_dist = teammate_msg[2].dist_to_ball;
//    int min_id = 2;

    //寻找对方onball
    opponent_msg[0]=opponent_msg[2];
    for(int i = 3; i <= 11; i++){
        if(opponent_msg[i].dist_to_ball<opponent_msg[0].dist_to_ball){
            opponent_msg[0]=opponent_msg[i];
        }
    }
    //寻找己方onball
    playerType tmpPlayer;
    tmpPlayer.dist_to_ball=100;
    for(int i = 2; i <= 11; i++){
        if(teammate_msg[i].dist_to_ball<tmpPlayer.dist_to_ball){
            tmpPlayer=teammate_msg[i];
        }
    }
    double near_rate = 0.95;
    //不变onball
    if(tmpPlayer.id==teammate_msg[0].id){
        teammate_msg[0]=tmpPlayer;
    }
    else {
        //改变onball
        if(tmpPlayer.dist_to_ball<teammate_msg[0].dist_to_ball*near_rate){
            teammate_msg[0]=tmpPlayer;
        }
        else{
            teammate_msg[0]=teammate_msg[teammate_msg[0].id];
        }
    }


//    if(tmpPlayer.id==teammate_msg[0].id){
//        //与上次onball相同
//        teammate_msg[0].onball_age++;
//        teammate_msg[0].offball_age=0;
//        teammate_msg[0].dist_to_ball=tmpPlayer.dist_to_ball;
////        return teammate_msg[0].id;
//    }
//    else if(teammate_msg[0].offball_age>=3){
//        for(int i = 2; i <= 11; i++){
//            if(teammate_msg[i].id==tmpPlayer.id){
//                teammate_msg[i].onball_age++;
//                if(teammate_msg[i].onball_age>=4){
//                    teammate_msg[i].offball_age=0;
//                    teammate_msg[0]=teammate_msg[i];
//                    cout<<"change====================================================\n";
//                }
//            }
//            else{
//                teammate_msg[i].onball_age=0;
//            }
//        }
////        return teammate_msg[0].id;
//    }
//    else{
//        teammate_msg[0].offball_age++;
//        for(int i = 2; i <= 11; i++){
//            if(teammate_msg[i].id==tmpPlayer.id){
//                teammate_msg[i].onball_age++;
//            }
//            else{
//                teammate_msg[i].onball_age=0;
//            }
//        }
//    }

    cout << "onball: " << teammate_msg[0].id << "  dist:" << teammate_msg[0].dist_to_ball << "  opponball: " << opponent_msg[0].id << endl;
    return teammate_msg[0].id;
//    if(numIDs[0]!=1){
//        cout << "onball: " << numIDs[0] << "  dist:" << dist[0] << endl;
//        return numIDs[0];
//    }
//    else {
//        cout << "onball: " << numIDs[1] << endl;
//        return numIDs[1];
//    }
    return 11;	//示例，返回11号，有需要请自行修改；
}


//示例阵型

vector<VecPosition> NaoBehavior::demoMode_1(VecPosition ball)
{
    //示例，有需要请自行修改
    deam_position.clear();
    float x = ball.getX();
    float y = ball.getY();
    VecPosition onballPos = worldModel->getTeammate(findClosestPlayer2Ball());

    deam_position.push_back(VecPosition(-14,y+8,0));
    deam_position.push_back(VecPosition(-13.8,y-5,0));
    deam_position.push_back(VecPosition(-12.5,y,0));
    deam_position.push_back((onballPos+VecPosition(-14,9,0))/2);
    deam_position.push_back((onballPos+VecPosition(-14,-9,0))/2);
    deam_position.push_back(VecPosition(x-2,y+0.75,0));
    deam_position.push_back(VecPosition(x-2,y-0.75,0));
    deam_position.push_back(VecPosition(x+5,y+2,0));
    deam_position.push_back(VecPosition(x+5,y-2,0));

    //检测是否越界
    for(int i=0;i<=(int)deam_position.size();i++)
    {
        if(deam_position[i].getY() >= 0.0)
        {
            if(deam_position[i].getX()  >= 15.0 && deam_position[i].getY() <10.0)
            {
                deam_position[i].setX(14.0);
            }
            else if(deam_position[i].getX()  < 15.0 && deam_position[i].getY() >=10.0)
            {
                deam_position[i].setY(9.5);
            }
            else if(deam_position[i].getX()  >= 15.0 && deam_position[i].getY() >=10.0)
            {
                deam_position[i].setX(14.0);
                deam_position[i].setY(9.5);
            }
        }
        else
        {
            if(deam_position[i].getX()  >= 15.0 && deam_position[i].getY() <=10.0)
            {
                deam_position[i].setX(14.0);
            }
            else if(deam_position[i].getX()  < 15.0 && deam_position[i].getY() <=-10.0)
            {
                deam_position[i].setY(-9.5);
            }
            else if(deam_position[i].getX()  >= 15.0 && deam_position[i].getY() <=-10.0)
            {
                deam_position[i].setX(14.0);
                deam_position[i].setY(-9.5);
            }
        }
    }
    //drawing beam
    worldModel->getRVSender()->clearStaticDrawings();
    if(worldModel->getUNum() == findClosestPlayer2Ball())  //离球最近的人定位的阵型
    {
        worldModel->getRVSender()->clear();
        worldModel->getRVSender()->clearStaticDrawings();
        for(int j=0;j<9;j++)
        {
            worldModel->getRVSender()->drawPoint(deam_position[j].getX(),deam_position[j].getY(),6.0f,RVSender::YELLOW);
        }
    }
    return deam_position;
}


vector<VecPosition> NaoBehavior::demoMode_2(VecPosition ball)
{
    //示例，有需要请自行修改
    deam_position.clear();
    float x = ball.getX();
    float y = ball.getY();

    deam_position.push_back(VecPosition(-13.2,y+0.5,0));
    deam_position.push_back(VecPosition(-13.2,y,0));
    deam_position.push_back(VecPosition(-13.2,y-0.5,0));
    deam_position.push_back(VecPosition(x-1,y+0.5,0));
    deam_position.push_back(VecPosition(x-1,y-0.5,0));
    deam_position.push_back(VecPosition(x+0.5,y+1.5,0));
    deam_position.push_back(VecPosition(x+0.5,y-1.5,0));
    deam_position.push_back(VecPosition(x+11,(y+10)/2,0));
    deam_position.push_back(VecPosition(x+13,(y-10)/2,0));


//    for(int i=0;i<9;i++){
//        deam_position.push_back(VecPosition(-7.5,-4+i,0));
//    }

    //检测是否越界
    for(int i=0;i<=(int)deam_position.size();i++)
    {
        if(deam_position[i].getY() >= 0.0)
        {
            if(deam_position[i].getX()  >= 15.0 && deam_position[i].getY() <10.0)
            {
                deam_position[i].setX(14.0);
            }
            else if(deam_position[i].getX()  < 15.0 && deam_position[i].getY() >=10.0)
            {
                deam_position[i].setY(9.5);
            }
            else if(deam_position[i].getX()  >= 15.0 && deam_position[i].getY() >=10.0)
            {
                deam_position[i].setX(14.0);
                deam_position[i].setY(9.5);
            }
        }
        else
        {
            if(deam_position[i].getX()  >= 15.0 && deam_position[i].getY() <=10.0)
            {
                deam_position[i].setX(14.0);
            }
            else if(deam_position[i].getX()  < 15.0 && deam_position[i].getY() <=-10.0)
            {
                deam_position[i].setY(-9.5);
            }
            else if(deam_position[i].getX()  >= 15.0 && deam_position[i].getY() <=-10.0)
            {
                deam_position[i].setX(14.0);
                deam_position[i].setY(-9.5);
            }
        }
    }
    //drawing beam
    worldModel->getRVSender()->clearStaticDrawings();
    if(worldModel->getUNum() == findClosestPlayer2Ball())  //离球最近的人定位的阵型
    {
        worldModel->getRVSender()->clear();
        worldModel->getRVSender()->clearStaticDrawings();
        for(int j=0;j<9;j++)
        {
            worldModel->getRVSender()->drawPoint(deam_position[j].getX(),deam_position[j].getY(),6.0f,RVSender::YELLOW);
        }
    }
    return deam_position;
}












