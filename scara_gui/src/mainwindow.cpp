#include "../include/scara_gui/mainwindow.h"
#include "ui_mainwindow.h"


double teachedPositions[HEIGHT][WIDTH];


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int argc;
    char **argv;
    ros::init(argc, argv, "scara_gui_node");
    //ros::NodeHandle n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25;
    //ros::NodeHandle nn1,nn2,nn3,nn4,nn5,nn6,nn7,nn8,nn9,nn10,nn11,nn12;
    ros::NodeHandle n,nn;
    ros::Rate loop_rate(10);

    ROS_INFO("spinner for GUI start\n");
    aspinner = new ros::AsyncSpinner(2);
    aspinner->start();

    ROS_WARN("Init publishers:");
    //Publishers
    jointControl_pub = n.advertise<geometry_msgs::PointStamped>("jointControl",1000);
    ROS_INFO("jointControl");
    positionControl_pub = n.advertise<geometry_msgs::Point>("positionControl",1000);
    ROS_INFO("positionControl");
    demo_pub = n.advertise<std_msgs::Bool>("demoControl",1000);                //nepouziva sa potom ho dat prec!!!!
    ROS_INFO("demoControl");
    getInfo_pub = n.advertise<std_msgs::Bool>("getInfo",1000);
    ROS_INFO("getInfo");
    setVel_pub = n.advertise<std_msgs::Float64>("setVelocity",1000);
    ROS_INFO("setVelocity");
    setAcc_pub = n.advertise<std_msgs::Float64>("setAcceleration",1000);
    ROS_INFO("setAcceleration");
    setPlanTime_pub = n.advertise<std_msgs::Float64>("setPlanningTime",1000);
    ROS_INFO("setPlanningTime");
    setNumOfAttempts_pub = n.advertise<std_msgs::Int32>("setNumberOfAttempts",1000);
    ROS_INFO("setNumberOfAttempts");
    gripperState_pub = n.advertise<std_msgs::Bool>("gripperState",1000);
    ROS_INFO("gripperState");
    start_pub = n.advertise<std_msgs::Bool>("startState",1000);
    ROS_INFO("startState");
    mode_pub = n.advertise<std_msgs::Int32>("modeSelectGUI",1000);
    ROS_INFO("modeSelectGUI");
    teachMode_pub = n.advertise<std_msgs::Int32>("teachModeGUI",1000);
    ROS_INFO("teachModeGUI");
    teachMode_startState = n.advertise<std_msgs::Bool>("teachModeStartState",1000);
    ROS_INFO("teachModeStartState");
    setTorq_pub = n.advertise<std_msgs::Float64>("setTorque",1000);
    ROS_INFO("setTorque");
    centralStop_pub = n.advertise<std_msgs::Int32>("centralStop",1000);
    ROS_INFO("centralStop");
    moveitMode_pub = n.advertise<std_msgs::Bool>("moveitModeStart",1000);
    ROS_INFO("moveitModeStart");
    colObjArrows_pub = n.advertise<std_msgs::Int32>("colisionObjectMovement",1000);
    ROS_INFO("colisionObjectMovement");
    displayRealColObj_pub = n.advertise<std_msgs::Bool>("displayRealColisionObject",1000);
    ROS_INFO("displayRealColisionObject");
    displayCustomColObj_pub = n.advertise<std_msgs::Bool>("displayCustomColisionObject",1000);
    ROS_INFO("displayCustomColisionObject");
    setPrecision_pub = n.advertise<std_msgs::Float64>("setPrecision",1000);
    ROS_INFO("setPrecision");
    setCustomObjPos_pub = n.advertise<geometry_msgs::Point>("CustomObjectPosition",1000);
    ROS_INFO("CustomObjectPosition");
    setCustomColObjSize_pub = n.advertise<geometry_msgs::Point>("CustomObjectSize",1000);
    ROS_INFO("CustomObjectSize");
    setRealColObjSize_pub = n.advertise<geometry_msgs::Point>("RealObjectSize",1000);
    ROS_INFO("RealObjectSize");

    ROS_INFO(".........................................");

    //Subscribers
    //demo_rviz.launch
    //jointStates_sub = nn1.subscribe("move_group/fake_controller_joint_states",1000,&MainWindow::jointStatesCallback, this);
    //ROS_INFO("Init subscriber jointStates");
    //demo_matlab_mfile
    ROS_WARN("Init subscribers:");
    //Pre realnu scaru
//    jointStates_sub = nn1.subscribe("scara_jointStates",1000,&MainWindow::jointStatesCallback, this);
//    ROS_INFO("scara_jointStates");
    //Pre simulaciu
    jointStates_sub = nn.subscribe("planned_poses_and_velocities",1000,&MainWindow::posesAndVelocitiesCallback, this);
    ROS_INFO("planned_poses_and_velocities");
    //actualAcc_sub = nn2.subscribe("actualAcceleration",1000,&MainWindow::actualAccCallback, this);    //zrychlenia od realnej scary
    actualAcc_sub = nn.subscribe("/planned_accelerations",1000,&MainWindow::actualAccCallback, this);  //Simulacia
    ROS_INFO("planned_accelerations");
    getInfo_sub = nn.subscribe("getInfoValues",1000,&MainWindow::getInfoCallback, this);
    ROS_INFO("getInfoValues");
    actualPose_sub = nn.subscribe("actualPose",1000,&MainWindow::actualPoseCallback, this);
    ROS_INFO("actualPose");
    errorMessage_sub = nn.subscribe("errorCode",1000,&MainWindow::errorCodeCallback, this);
    ROS_INFO("errorCode");
    gripperCommand_sub = nn.subscribe("gripperCommand",1000,&MainWindow::gripperCommandCallback, this);
    ROS_INFO("gripperCommand");
    pushButton_sub = nn.subscribe("scara_pushbutton",1000,&MainWindow::pushButtonCallback, this);
    ROS_INFO("scara_pushbutton");
    lightBarrier_sub = nn.subscribe("scara_lightbarrier",1000,&MainWindow::lightBarrierCallback, this);
    ROS_INFO("scara_lightbarrier");
    desiredPose_sub = nn.subscribe("desiredPoseGUI",1000,&MainWindow::desiredPoseCallback, this);
    ROS_INFO("desiredPoseGUI");
    //shit_sub = nn10.subscribe("hovadina",1000,&MainWindow::kktinaCallback, this);
    moveit_jointStates = nn.subscribe("joint_states",1000, &MainWindow::moveitJointStatesCallback, this);
    ROS_INFO("joint_states");
    torqJ1_sub = nn.subscribe("torque_J1",1000, &MainWindow::torqueJ1Callback, this); //simulacia
    ROS_INFO("torque_J1");
    torqJ2_sub = nn.subscribe("torque_J2",1000, &MainWindow::torqueJ2Callback, this); //simulacia
    ROS_INFO("torque_J2\n");
    ROS_WARN("GUI init complete");

    ROS_WARN("Init shared variables");
    init_shared_variables();
    ROS_WARN("Shared variables init complete");

    sleep(2);

    ROS_WARN("Init paralel thread");
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::displayValues);
    timer->start(100);
    ROS_WARN("Paralel thread init complete");

}

MainWindow::~MainWindow()
{
    delete ui;
}




//**************************** JOINT CONTROL ***************************************//
void MainWindow::on_jointControl_Start_PushButton_3_clicked(){
    //Display on GUI
    ui->jointControl_J1_LineEdit->setText(QString::number(ui->jointControl_J1_Slider_3->value() / 100.0) + "rad");
    ui->jointControl_J2_LineEdit->setText(QString::number(ui->jointControl_J2_Slider_3->value() / 100.0) + "rad");
    ui->jointControl_J3_LineEdit->setText(QString::number(ui->jointControl_J3_Slider_3->value()) + "cm");
    ui->status_gripper_OnOff_3->display(gripperState);

    //Send to ROS
    jointControl_Values_msg.point.x = ui->jointControl_J1_Slider_3->value() / 100.0;
    jointControl_Values_msg.point.y = ui->jointControl_J2_Slider_3->value() / 100.0;
    //jointControl_Values_msg.point.z = -(ui->jointControl_J3_Slider_3->value() / 100.0) + 0.04;
    jointControl_Values_msg.point.z = (ui->jointControl_J3_Slider_3->value() / 100.0);
    if (gripperState)
        gripperState_msg.data = true;
    else
        gripperState_msg.data = false;
    startState_msg.data = true;

    for (int i=0;i<100;i++){
        jointControl_pub.publish(jointControl_Values_msg);
        gripperState_pub.publish(gripperState_msg);
        start_pub.publish(startState_msg);
    }

}

void MainWindow::on_jointControl_J1_Slider_3_actionTriggered(int action){
    ui->jointControl_J1_LineEdit->setText(QString::number(ui->jointControl_J1_Slider_3->value() / 100.0) + "rad");

}

void MainWindow::on_jointControl_J2_Slider_3_actionTriggered(int action){
    ui->jointControl_J2_LineEdit->setText(QString::number(ui->jointControl_J2_Slider_3->value() / 100.0) + "rad");
}

void MainWindow::on_jointControl_J3_Slider_3_actionTriggered(int action){
    ui->jointControl_J3_LineEdit->setText(QString::number(ui->jointControl_J3_Slider_3->value()) + "cm");
}

void MainWindow::on_jointControl_Gripper_Checkbox_3_toggled(bool checked){
    //GUI
    if (checked){
        ui->jointControl_gripper_LineEdit->setText("ON!");
    }else{
        ui->jointControl_gripper_LineEdit->setText("OFF!");
    }
    gripperState = checked;
    ui->status_gripper_OnOff_3->display(gripperState);

    //ROS
    if (checked)
        gripperState_msg.data = true;
    else
        gripperState_msg.data = false;

    for (int i=0;i<100;i++){
        gripperState_pub.publish(gripperState_msg);
    }

}

void MainWindow::on_jointControl_Stop_PushButton_4_clicked(){

    startState_msg.data = false;
    for (int i=0;i<100;i++){
        start_pub.publish(startState_msg);
    }

}

void MainWindow::on_jointControl_Reset_PushButton_3_clicked(){
    //GUI
    ui->jointControl_J1_Slider_3->setValue(0);
    ui->jointControl_J2_Slider_3->setValue(0);
    ui->jointControl_J3_Slider_3->setValue(0);
    ui->jointControl_J1_LineEdit->setText(QString::number(0.0) + "rad");
    ui->jointControl_J2_LineEdit->setText(QString::number(0.0) + "rad");
    ui->jointControl_J3_LineEdit->setText(QString::number(0.0) + "cm");
    ui->jointControl_Gripper_Checkbox_3->setChecked(false);
    ui->jointControl_gripper_LineEdit->setText("OFF!");

    //ROS
    //Send to ROS
    jointControl_Values_msg.point.x = 0.0;
    jointControl_Values_msg.point.y = 0.0;
    //jointControl_Values_msg.point.z = 0.04;
    jointControl_Values_msg.point.z = 0.0;
    gripperState_msg.data = false;
    startState_msg.data = true;
    for (int i=0;i<100;i++){
        jointControl_pub.publish(jointControl_Values_msg);
        gripperState_pub.publish(gripperState_msg);
        start_pub.publish(startState_msg);
    }


}

void MainWindow::on_jointControl_collision_checkbox_toggled(bool checked){

    //ROS
    dispRealObj_msg.data = checked;
    for (int i=0;i<100;i++){
        displayRealColObj_pub.publish(dispRealObj_msg);
    }

}
//................................................................................//






//***************************** Position Control custom **************************//
void MainWindow::on_positionControlCustom_Start_PushButton_3_clicked(){
    //ROS
    positionControl_Values_msg.x = ui->positionControlCustom_X_LineEdit_3->text().toDouble();
    positionControl_Values_msg.y = ui->positionControlCustom_Y_LineEdit_3->text().toDouble();
    positionControl_Values_msg.z = ui->positionControlCustom_Z_LineEdit_3->text().toDouble();
    if (gripperState)
        gripperState_msg.data = true;
    else
        gripperState_msg.data = false;
    startState_msg.data = true;

    for (int i=0;i<100;i++){
        positionControl_pub.publish(positionControl_Values_msg);
        gripperState_pub.publish(gripperState_msg);
        start_pub.publish(startState_msg);
    }



}

void MainWindow::on_positionControlCustom_Stop_PushButton_4_clicked(){

    startState_msg.data = false;
    for (int i=0;i<100;i++){
        start_pub.publish(startState_msg);
    }
}

void MainWindow::on_positionControlCustom_Reset_PushButton_5_clicked(){
    //GUI
    ui->positionControlCustom_X_LineEdit_3->setText(QString::number(0.704));
    ui->positionControlCustom_Y_LineEdit_3->setText(QString::number(0.58));
    ui->positionControlCustom_Z_LineEdit_3->setText(QString::number(1.04));
    ui->positionControl_Gripper_Checkbox_4->setChecked(false);
    //ROS
    //Start Pose of SCARA
    positionControl_Values_msg.x = 0.7;
    positionControl_Values_msg.y = 0.57;
    positionControl_Values_msg.z = 1.04;
    gripperState_msg.data = false;
    startState_msg.data = true;
    for (int i=0;i<100;i++){
        positionControl_pub.publish(positionControl_Values_msg);
        gripperState_pub.publish(gripperState_msg);
        start_pub.publish(startState_msg);
    }
}

void MainWindow::on_positionControl_Gripper_Checkbox_4_toggled(bool checked){

    //GUI
    ui->status_gripper_OnOff_3->display(checked);

    gripperState = checked;
    //ROS
    if (checked)
        gripperState_msg.data = true;
    else
        gripperState_msg.data = false;

    for (int i=0;i<100;i++){
        gripperState_pub.publish(gripperState_msg);
    }
}

void MainWindow::on_positionControlCustom_collision_checkbox_toggled(bool checked){

    //ROS
    dispRealObj_msg.data = checked;
    for (int i=0;i<100;i++){
        displayRealColObj_pub.publish(dispRealObj_msg);
    }

}
//...............................................................................//





//******************************* DEMO APK *************************************//
void MainWindow::on_positionControl2_Start_PushButton_3_clicked(){
    //GUI
    ui->positionControl2_LineEdit->setText("DEMO application 2 RUNNING!");

    //ROS
    if (gripperState)
        gripperState_msg.data = true;
    else
        gripperState_msg.data = false;
    startState_msg.data = true;

    for (int i=0;i<100;i++){
        gripperState_pub.publish(gripperState_msg);
        start_pub.publish(startState_msg);
    }

}

void MainWindow::on_positionControl2_Stop_PushButton_3_clicked(){
    //GUI
    ui->positionControl2_LineEdit->setText("DEMO application 2 STOPPED!");

    //ROS
    startState_msg.data = false;
    for (int i=0;i<100;i++){
        start_pub.publish(startState_msg);
    }
}

void MainWindow::on_positionControl2_collision_checkbox_toggled(bool checked){

    //ROS
    dispRealObj_msg.data = checked;
    for (int i=0;i<100;i++){
        displayRealColObj_pub.publish(dispRealObj_msg);
    }

}
//...............................................................................//






//******************************* TEACH MODE ************************************//
void MainWindow::on_teachMode_teachButton_clicked(){
    QString currentMode;
    ROS_INFO("teach !");
    positionControl_Values_msg.x = ui->teachMode_Xpos_lineEdit->text().toDouble();
    positionControl_Values_msg.y = ui->teachMode_Ypos_lineEdit->text().toDouble();
    positionControl_Values_msg.z = ui->teachMode_Zpos_lineEdit->text().toDouble();

    if (teachModeIndex == 0){
        ui->teachMode_info_textEdit->setText("X = " + QString::number(positionControl_Values_msg.x) + " Y= " +
                                             QString::number(positionControl_Values_msg.y) + " Z= " +
                                             QString::number(positionControl_Values_msg.z) + " ] \n" + "Operation type = Pick");
        ui->teachMode_modeDisplay_lcdnumber->display(0.0);
    }else if(teachModeIndex%2 == 0){
        ui->teachMode_info_textEdit->setText("[ X = " + QString::number(positionControl_Values_msg.x) + " Y= " +
                                             QString::number(positionControl_Values_msg.y) + " Z= " +
                                             QString::number(positionControl_Values_msg.z) + " ] \n" + "Operation type = Pick");
        ui->teachMode_modeDisplay_lcdnumber->display(0.0);
    }else{
        ui->teachMode_info_textEdit->setText("[ X = " + QString::number(positionControl_Values_msg.x) + " Y= " +
                                             QString::number(positionControl_Values_msg.y) + " Z= " +
                                             QString::number(positionControl_Values_msg.z) + " ] \n" + "Operation type = Place");
        ui->teachMode_modeDisplay_lcdnumber->display(1.0);
    }
    positionControl_pub.publish(positionControl_Values_msg);


    teachModeIndex++;

    startState_msg.data = true;
    for (int i=0;i<100;i++){
        start_pub.publish(startState_msg);
        positionControl_pub.publish(positionControl_Values_msg);

    }
}

void MainWindow::on_teachMode_stopTeachButton_clicked(){
    ROS_INFO("stop teach !");
    startState_msg.data = false;
    for (int i=0;i<100;i++){
        start_pub.publish(startState_msg);
    }
}

void MainWindow::on_teachMode_tabWidget_tabBarClicked(int index){

    ROS_INFO("tab changed !");
    teachModeSelect_msg.data = index;
    ROS_INFO("tab number %d",teachModeSelect_msg.data);
    ui->teachMode_modeDisplay_lcdnumber->display(1.0);

    for (int i=0;i<100;i++){
       teachMode_pub.publish(teachModeSelect_msg);
    }


}

void MainWindow::on_teachModeRun_start_pushbutton_clicked(){
    ROS_INFO("teach mode START !");
    ui->teachModeRun_info_lineEdit->setText("TEACH application 2 RUNNING!");


    teachModeState_msg.data = true;
        for (int i=0;i<100;i++){
        teachMode_startState.publish(teachModeState_msg);
    }
}

void MainWindow::on_teachModeRun_stop_pushbutton_clicked(){
    ROS_INFO("teach mode STOP !");
    ui->teachModeRun_info_lineEdit->setText("TEACH application 2 STOPPED!");
    teachModeState_msg.data = false;

    for (int i=0;i<100;i++){
        teachMode_startState.publish(teachModeState_msg);
    }
}

void MainWindow::on_teachModeRun_collisiongui_checkbox_toggled(bool checked){

    //ROS
    dispRealObj_msg.data = checked;
    for (int i=0;i<100;i++){
        displayRealColObj_pub.publish(dispRealObj_msg);
    }

}
//...............................................................................//





//****************************** TEACH MODE BY HAND ******************************//
void MainWindow::on_teachMode_teachButtonHand_4_clicked(){

    QString currentMode;


    //if ((actualJointStates.position[0] != lastValueJ1) || (actualJointStates.position[1] != lastValueJ2) || (actualJointStates.position[2] != lastValueJ3)){  //Find out if there was a change
        lastValueJ1 = actualJointStates.position[0];
        lastValueJ2 = actualJointStates.position[1];
        lastValueJ3 = actualJointStates.position[2];
        if (j<HEIGHT){
            teachedPositions[j][0] = actualJointStates.position[0];
            teachedPositions[j][1] = actualJointStates.position[1];
            teachedPositions[j][2] = actualJointStates.position[2];
            j++;

            ROS_INFO("teach !");
            jointControl_Values_msg.point.x = actualJointStates.position[0];
            jointControl_Values_msg.point.y = actualJointStates.position[1];
            jointControl_Values_msg.point.z = actualJointStates.position[2];

            if (teachModeIndexHand == 0){
                ui->teachMode_infoHand_textEdit_4->setText("J1= " + QString::number(jointControl_Values_msg.point.x) + " J2= " +
                                                           QString::number(jointControl_Values_msg.point.y) + " J3= " +
                                                           QString::number(jointControl_Values_msg.point.z) + " ] \n" + "Operation type = Pick");
                ui->teachMode_modeDisplayHand_lcdnumber_->display(0.0);
            }else if(teachModeIndexHand%2 == 0){
                ui->teachMode_infoHand_textEdit_4->setText("[ J1 = " + QString::number(jointControl_Values_msg.point.x) + " J2= " +
                                                           QString::number(jointControl_Values_msg.point.y) + " J3= " +
                                                           QString::number(jointControl_Values_msg.point.z) + " ] \n" + "Operation type = Pick");
                ui->teachMode_modeDisplayHand_lcdnumber_->display(0.0);
            }else{
                ui->teachMode_infoHand_textEdit_4->setText("[ J1= " + QString::number(jointControl_Values_msg.point.x) + " J2= " +
                                                           QString::number(jointControl_Values_msg.point.y) + " J3= " +
                                                           QString::number(jointControl_Values_msg.point.z) + " ] \n" + "Operation type = Place");
                ui->teachMode_modeDisplayHand_lcdnumber_->display(1.0);
            }

            teachModeIndexHand++;
            startState_msg.data = true;
            for (int i=0;i<100;i++){
                start_pub.publish(startState_msg);
                jointControl_pub.publish(jointControl_Values_msg);
            }
        }else{
            ui->error_lineEdit->setText("Allowed max." + QString::number(HEIGHT)+ " pos!!! New position wont be registered");
        }



    //}else{
    //    ui->teachMode_infoHand_textEdit_4->setText("You entered the same values twice in a ROW!! \n These joint values wont be registered!!");
    //}



}

void MainWindow::on_teachMode_stopTeachButtonHand_4_clicked(){

    ROS_INFO("stop teach !");
    startState_msg.data = false;
    for (int i=0;i<100;i++){
        start_pub.publish(startState_msg);
    }
}

void MainWindow::on_teachModeRun_startHand_pushbutton_4_clicked(){

    ROS_INFO("teach mode START !");
    ui->teachModeRun_infoHand_lineEdit_4->setText("TEACH HAND application 2 RUNNING!");


    teachModeState_msg.data = true;
    for (int i=0;i<100;i++){
        teachMode_startState.publish(teachModeState_msg);
    }
}

void MainWindow::on_teachModeRun_stopHand_pushbutton_4_clicked(){

    ROS_INFO("teach mode STOP !");
    ui->teachModeRun_infoHand_lineEdit_4->setText("TEACH HAND application 2 STOPPED!");
    teachModeState_msg.data = false;

    for (int i=0;i<100;i++){
        teachMode_startState.publish(teachModeState_msg);
    }
}

void MainWindow::on_teachMode_tabWidget_2_tabBarClicked(int index){

    bool deleted = false;
    ROS_INFO("tab changed !");
    teachModeSelect_msg.data = index;
    ROS_INFO("tab number %d",teachModeSelect_msg.data);

    for (int i=0;i<100;i++){
        teachMode_pub.publish(teachModeSelect_msg);
    }

    ui->teachMode_teachedPositions_textEdit_4->clear();
    if (index == 1){
        //vypisat naucene pozicie,...
        if (j%2 == 1){
            j--;
            deleted = true;
        }
        for (int i=0;i<j;i++){
            if (i == 0){
                ui->teachMode_teachedPositions_textEdit_4->append("Pos." + QString::number(i) + "   x=" + QString::number(teachedPositions[i][0]) +
                " y=" + QString::number(teachedPositions[i][1]) + " z=" + QString::number(teachedPositions[i][2]) + "    PICK");

            }else if (i%2 == 0){
                ui->teachMode_teachedPositions_textEdit_4->append("Pos." + QString::number(i) + "   x=" + QString::number(teachedPositions[i][0]) +
                " y=" + QString::number(teachedPositions[i][1]) + " z=" + QString::number(teachedPositions[i][2]) + "    PICK");
            }else{
                ui->teachMode_teachedPositions_textEdit_4->append("Pos." + QString::number(i) + "   x=" + QString::number(teachedPositions[i][0]) +
                " y=" + QString::number(teachedPositions[i][1]) + " z=" + QString::number(teachedPositions[i][2]) + "    PLACE");
            }
        }
        if (deleted){
            ui->teachMode_teachedPositions_textEdit_4->setTextColor( QColor( "red" ) );
            ui->teachMode_teachedPositions_textEdit_4->append("Last PICK position deleted due to not defined PLACE position");
            ui->teachMode_teachedPositions_textEdit_4->setTextColor( QColor( "black" ) );
            deleted = false;
        }

    }else if (index == 0){
        j=0;
    }
}

void MainWindow::on_teachModeRun_collision_checkbox_toggled(bool checked){

    //ROS
    dispRealObj_msg.data = checked;
    for (int i=0;i<100;i++){
        displayRealColObj_pub.publish(dispRealObj_msg);
    }
}
//...............................................................................//






//****************************** Movement in moveit *****************************//
void MainWindow::on_moveit_checkBox_toggled(bool checked){
    moveitMode_msg.data = checked;

    for (int i=0;i<100;i++){
        moveitMode_pub.publish(moveitMode_msg);
    }
    if (checked){
        ui->moveit_lineEdit->setText("Movement in Moveit! enabled");
    }else{
        ui->moveit_lineEdit->setText("Movement in Moveit! disabled");
    }

}

void MainWindow::on_moveit_gripper_checkBox_toggled(bool checked){

    //GUI
    gripperState = checked;
    ui->status_gripper_OnOff_3->display(gripperState);

    //ROS
    gripperState_msg.data = checked;
    for (int i=0;i<100;i++){
        gripperState_pub.publish(gripperState_msg);
    }

}
//...............................................................................//





//*************************** Get information ********************************//
void MainWindow::on_basicInfo_GetInfo_PushButton_3_clicked(){
    //ROS
    //Publish
    getInfoState_msg.data = true;
    //for (int i=0;i<100;i++) {
        getInfo_pub.publish(getInfoState_msg);
    //}

    ui->basicInfo_RobotModel_TextBrowser_3->setText("scara_arm");
    ui->basicInfo_ReferenceFrame_TextBrowser_3->setText("world");
    ui->basicInfo_EffectorLink_TextBrowser_3->setText("tool0");
    ui->basicInfo_ActiveJoints_TextBrowser_3->setText("Joint1 , Joint2, Joint_GripperBase");


}
//...............................................................................//




//*************************** Set information *******************************//
void MainWindow::on_setParameters_Torque_PushButton_3_clicked(){
    setParamFloat_msg.data = ui->setParameters_Torque_LineEdit_3->text().toFloat();
    for (int i=0;i<5;i++) {
        setTorq_pub.publish(setParamFloat_msg);
    }

}

void MainWindow::on_setParameters_Precision_PushButton_4_clicked(){

    setParamFloat_msg.data = ui->setParameters_Precision_LineEdit_4->text().toFloat();

    for (int i=0;i<5;i++) {
        setPrecision_pub.publish(setParamFloat_msg);
    }
}

void MainWindow::on_setParameters_Velocity_PushButton_3_clicked(){
    //ROS
    setParamFloat_msg.data = ui->setParameters_Velocity_LineEdit_3->text().toFloat();
    for (int i=0;i<5;i++) {
        setVel_pub.publish(setParamFloat_msg);
    }

}

void MainWindow::on_setParameters_Acceleration_PushButton_3_clicked(){
    //ROS
    setParamFloat_msg.data = ui->setParameters_Acceleration_LineEdit_3->text().toFloat();
    for (int i=0;i<5;i++) {
        setAcc_pub.publish(setParamFloat_msg);
    }
}

void MainWindow::on_setParameters_PlanningTime_PushButton_3_clicked(){
    //ROS
    setParamFloat_msg.data = ui->setParameters_PlanningTime_LineEdit_3->text().toFloat();
    for (int i=0;i<5;i++) {
        setPlanTime_pub.publish(setParamFloat_msg);
    }
}

void MainWindow::on_setParameters_NumOfAttempts_PushButton_3_clicked(){
    //ROS
    setParamInt_msg.data = ui->setParameters_NumOfAttempts_LineEdit_3->text().toInt();
    for (int i=0;i<5;i++) {
        setNumOfAttempts_pub.publish(setParamInt_msg);
    }
}
//...............................................................................//






//*************************** Colision object *******************************//
void MainWindow::on_colisionObject_CustomObj_checkButton_toggled(bool checked){

    //GUI
    if (checked){
        ui->colisionObject_CustomObj_lineEdit->setText("Enabled");
    }else{
        ui->colisionObject_CustomObj_lineEdit->setText("Disabled");
    }

    //ROS
    dispCustomObj_msg.data = checked;
    for (int i=0;i<100;i++){
        displayCustomColObj_pub.publish(dispCustomObj_msg);
    }




}

void MainWindow::on_colisionObject_RealObj_checkButton_toggled(bool checked){

    //GUI
    if (checked){
        ui->colisionObject_RealObj_lineEdit->setText("Enabled");
    }else{
        ui->colisionObject_RealObj_lineEdit->setText("Disabled");
    }

    //ROS
    dispRealObj_msg.data = checked;
    for (int i=0;i<100;i++){
        displayRealColObj_pub.publish(dispRealObj_msg);
    }

}

void MainWindow::on_colisionObject_CustomObj_posChangeenterpushButton_2_clicked(){

    posCustomObj_msg.x = ui->colisionObject_CustomObj_posXLineEdit_4->text().toFloat();
    posCustomObj_msg.y = ui->colisionObject_CustomObj_posYLineEdit_5->text().toFloat();
    posCustomObj_msg.z = 0.0;

    for (int i=0;i<10;i++){
        setCustomObjPos_pub.publish(posCustomObj_msg);
    }
}

void MainWindow::on_colisionObject_CustomObj_SizeenterpushButton_clicked(){

    sizeCustomObj_msg.x = ui->colisionObject_CustomObj_sizeXLineEdit_3->text().toFloat();
    sizeCustomObj_msg.y = ui->colisionObject_CustomObj_sizeYLineEdit_6->text().toFloat();
    sizeCustomObj_msg.z = 0.0;

    for (int i=0;i<10;i++){
        setCustomColObjSize_pub.publish(sizeCustomObj_msg);
    }
}

void MainWindow::on_colisionObject_RealObj_SizeenterpushButton_2_clicked(){

    sizeRealObj_msg.x = ui->colisionObject_RealObj_sizeXLineEdit_4->text().toFloat();
    sizeRealObj_msg.y = ui->colisionObject_RealObj_sizeYLineEdit_7->text().toFloat();
    sizeRealObj_msg.z = 0.0;

    for (int i=0;i<10;i++){
        setRealColObjSize_pub.publish(sizeRealObj_msg);
    }
}

void MainWindow::on_colisionObject_Reset_pushbutton_clicked(){

    arrows_msg.data = 0;
    for (int i=0;i<100;i++){
        colObjArrows_pub.publish(arrows_msg);
    }

}

void MainWindow::on_colisionObject_Up_pushbutton_clicked(){

    arrows_msg.data = 1;
    //for (int i=0;i<100;i++){
        colObjArrows_pub.publish(arrows_msg);
    //}
}

void MainWindow::on_colisionObject_Left_pushbutton_clicked(){

    arrows_msg.data = 2;
    //for (int i=0;i<100;i++){
        colObjArrows_pub.publish(arrows_msg);
    //}
}

void MainWindow::on_colisionObject_Down_pushbutton_clicked(){

    arrows_msg.data = 3;
    //for (int i=0;i<100;i++){
        colObjArrows_pub.publish(arrows_msg);
    //}
}

void MainWindow::on_colisionObject_Right_pushbutton_clicked(){

    arrows_msg.data = 4;
    //for (int i=0;i<100;i++){
        colObjArrows_pub.publish(arrows_msg);
    //}
}



//...............................................................................//


//******************************** Tab widget ***********************************//
void MainWindow::on_workingModes_3_tabBarClicked(int index){
    //GUI
    if (index == 0){
        ui->currentWorkingMode_LineEdit->setText("Info");
        modeSelect_msg.data = 0;
    }else{
        ui->currentWorkingMode_LineEdit->setText(QString::number(index));
        modeSelect_msg.data = index;
        if (index == 5){
            j = 0;
        }
    }

    //ROS
    for (int i=0;i<100;i++) {
        mode_pub.publish(modeSelect_msg);

    }

}
//...............................................................................//



//******************************* CENTRAL STOP **********************************//
void MainWindow::on_centralStop_clicked(){

    //kill matlab
    centralStop_msg.data = 1;
    for (int i = 0;i<100;i++){
        centralStop_pub.publish(centralStop_msg);
    }
    //kill roslaunch
    system("pkill roslaunch");

    ui->error_lineEdit->setText("CENTRAL STOP PUSHED! Matlab and ROS stopped!");

    //kill GUI
    sleep(3);
    QApplication::quit();


}
//...............................................................................//


//******************** Filter input values from scara ***************************//
bool MainWindow::filterValues(double inputValue) {

    if ((inputValue > -MIN_DISPLAY_VALUE) && (inputValue < MIN_DISPLAY_VALUE))
        return true;
    else
        return false;

}
//...............................................................................//






//********************** Callbacks **********************************************//
void MainWindow::jointStatesCallback(const sensor_msgs::JointState jointState){

    //Save current joint state -> for teach mode
    //ROS_INFO("new joint states!!!!");
//    actualJointStates = jointState;
//
//    //ROS_INFO("Joint states %f %f %f",jointState.position[1], jointState.position[2], jointState.position[3]);
//    ui->status_joint1pos_rad_3->display(jointState.position[0]*RAD_TO_DEG);
//    ui->status_joint2pos_rad_3->display(jointState.position[1]*RAD_TO_DEG);
//    ui->status_joint3pos_rad_3->display(jointState.position[2]*100.0);
//    ui->status_joint1pos_deg_3->display(jointState.position[0]);
//    ui->status_joint2pos_deg_3->display(jointState.position[1]);
//    ui->status_joint3pos_deg_3->display(jointState.position[2]*100.0);
//
//    //TEACH MODE
//    ui->teachModeHand_J1_LCD->display(jointState.position[0]*RAD_TO_DEG);
//    ui->teachModeHand_J2_LCD->display(jointState.position[1]*RAD_TO_DEG);
//    ui->teachModeHand_J3_LCD->display(jointState.position[2]*100.0);
//    //
//
//    //Neskor doplnit rychlosti a momenty
//    if (jointState.velocity.size() >= 3){
//        if (filterValues(jointState.velocity[0])){
//            ui->status_joint1vel_3->display(0.0);
//        }else{
//            ui->status_joint1vel_3->display(jointState.velocity[0]);
//        }
//
//        if (filterValues(jointState.velocity[1])){
//            ui->status_joint2vel_3->display(0.0);
//        }else{
//            ui->status_joint2vel_3->display(jointState.velocity[1]);
//        }
//
//        if (filterValues(jointState.velocity[2])){
//            ui->status_joint3vel_3->display(0.0);
//        }else{
//            ui->status_joint3vel_3->display(jointState.velocity[2]);
//        }
//
////        if ((jointState.velocity[0] > -MIN_DISPLAY_VALUE) && (jointState.velocity[0] < MIN_DISPLAY_VALUE))
////            ui->status_joint1vel_3->display(0.0);
////        else
////            ui->status_joint1vel_3->display(jointState.velocity[0]);
////
////        if ((jointState.velocity[1] > -MIN_DISPLAY_VALUE) && (jointState.velocity[1] < MIN_DISPLAY_VALUE))
////            ui->status_joint2vel_3->display(0.0);
////        else
////            ui->status_joint2vel_3->display(jointState.velocity[1]);
////
////        if ((jointState.velocity[2] > -MIN_DISPLAY_VALUE) && (jointState.velocity[2] < MIN_DISPLAY_VALUE))
////            ui->status_joint3vel_3->display(0.0);
////        else
////            ui->status_joint3vel_3->display(jointState.velocity[2]);
//
//    }else{
//        ui->status_joint1vel_3->display(0.00);
//        ui->status_joint2vel_3->display(0.00);
//        ui->status_joint3vel_3->display(0.00);
//    }
//
//    //ROS_INFO("actual torque: J1 = %f J2 = %f", jointState.effort[0], jointState.effort[1]);
//    if (jointState.effort.size() >= 3){
//
//        if (filterValues(jointState.effort[0])){
//            ui->status_joint1torq_3->display(0.0);
//        }else{
//            ui->status_joint1torq_3->display(jointState.effort[0]);
//        }
//
//        if (filterValues(jointState.effort[1])){
//            ui->status_joint2torq_3->display(0.0);
//        }else{
//            ui->status_joint2torq_3->display(jointState.effort[1]);
//        }
//
////        ui->status_joint1torq_3->display(jointState.effort[0]);
////        ui->status_joint2torq_3->display(jointState.effort[1]);
//        ui->status_joint3torq_3->display(0.00);
//    }else{
//        ui->status_joint1torq_3->display(0.00);
//        ui->status_joint2torq_3->display(0.00);
//        ui->status_joint3torq_3->display(0.00);
//    }
//
//    ui->status_pose_Z->display(1.04-jointState.position[2]);
//    ui->status_pose_Zdesired->display(1.04-jointState.position[2]);//////

}

void MainWindow::posesAndVelocitiesCallback(const geometry_msgs::Pose poseAndVelocity){

    disp_curr_joint_vel[0] = poseAndVelocity.orientation.x;
    disp_curr_joint_vel[1] = poseAndVelocity.orientation.y;
    disp_curr_joint_vel[2] = poseAndVelocity.orientation.z;

}

void MainWindow::jointControlCallback(const geometry_msgs::PointStamped pointStamped){
    //ROS_INFO("joint control callback");
}

void MainWindow::getInfoCallback(const scara_msgs::robot_info robotInfo){

    //ROS_INFO("robot info callback");
    std::string robot_model = robotInfo.robot_model;
    std::string reference_frame = robotInfo.reference_frame;
    std::string effector_link = robotInfo.efector_link;
    std::string active_joints = robotInfo.active_joints;
    ROS_INFO_STREAM(robot_model);
    ROS_INFO_STREAM(reference_frame);
    ROS_INFO_STREAM(effector_link);
    ROS_INFO_STREAM(active_joints);

    disp_rob_model = robot_model;
    disp_refer_frame = reference_frame;
    disp_effect_link = effector_link;
    disp_active_joints = active_joints;

    disp_curr_rob_status[0] = robotInfo.position_x;
    disp_curr_rob_status[1] = robotInfo.position_y;
    disp_curr_rob_status[2] = robotInfo.position_z;
    disp_curr_rob_status[3] = robotInfo.orientation_x;
    disp_curr_rob_status[4] = robotInfo.orientation_y;
    disp_curr_rob_status[5] = robotInfo.orientation_z;
    disp_curr_rob_status[6] = robotInfo.orientation_w;
}   //String to QString !!!!!!

void MainWindow::actualPoseCallback(const geometry_msgs::Pose pose){

    disp_curr_carthesian_pos[0] = pose.position.x;
    disp_curr_carthesian_pos[1] = pose.position.y;
    disp_curr_carthesian_pos[2] = pose.position.z;
}

void MainWindow::actualAccCallback(const geometry_msgs::Point accValues){

    disp_curr_joint_acc[0] = accValues.x;
    disp_curr_joint_acc[1] = accValues.y;
    disp_curr_joint_acc[2] = accValues.z;
}

void MainWindow::errorCodeCallback(const std_msgs::Int32 errorCode){

    disp_err_code = errorCode.data;
}

void MainWindow::kktinaCallback(const geometry_msgs::Pose pose){

    ROS_INFO("kktina");
}

void MainWindow::pushButtonCallback(const std_msgs::Byte pushButtonState){

    disp_push_button = pushButtonState.data;
}

void MainWindow::lightBarrierCallback(const std_msgs::Byte lightBarrierState){

    disp_light_barrier = lightBarrierState.data;
}

void MainWindow::gripperCommandCallback(const std_msgs::Byte gripperCommandState){

    disp_gripper_state = gripperCommandState.data;
}

void MainWindow::desiredPoseCallback(const geometry_msgs::Point desiredPose){

    disp_des_carthesian_pos[0] = desiredPose.x;
    disp_des_carthesian_pos[1] = desiredPose.y;
    disp_des_carthesian_pos[2] = desiredPose.z;
}

void MainWindow::moveitJointStatesCallback(const sensor_msgs::JointState jointState){

    //ROS_INFO("Current joint states from moveit J1=%f J2=%f J3 =%f", jointState.position[0], jointState.position[1], jointState.position[2]);
    actualJointStates = jointState;

    //Len pre SCARU
//    disp_curr_joint_pos_DEG[0] = jointState.position[0]*RAD_TO_DEG;
//    disp_curr_joint_pos_DEG[1] = jointState.position[1]*RAD_TO_DEG;
//    disp_curr_joint_pos_DEG[2] = jointState.position[2]*10;

    //Pre SCARU s RT
    disp_curr_joint_pos_DEG[0] = jointState.position[1]*RAD_TO_DEG;
    disp_curr_joint_pos_DEG[1] = jointState.position[2]*RAD_TO_DEG;
    disp_curr_joint_pos_DEG[2] = jointState.position[3]*10;

}

void MainWindow::torqueJ1Callback(const std_msgs::Float64 torque){

    disp_curr_joint_torq[0] = torque.data;
}

void MainWindow::torqueJ2Callback(const std_msgs::Float64 torque){

    disp_curr_joint_torq[1] = torque.data;
}

void MainWindow::displayValues(){

    static double last_joint_pos_DEG[] = {999.9,999.9,999.9}, last_joint_vel[] = {999.9,999.9,999.9}, last_joint_acc[] = {999.9,999.9,999.9}, last_joint_torq[] = {999.9,999.9,999.9};
    static double last_des_carthesian_pos[] = {999.9,999.9,999.9}, last_carthesian_pos[] = {999.9,999.9,999.9}, last_rob_status[] = {999.9,999.9,999.9, 999.9,999.9,999.9, 999.9};
    static uint8_t last_gripper_state = 0, last_light_barrier= 0, last_push_button=0, init=0, last_err_code = -1;
    std::string last_rob_model="-1", last_refer_frame = "-1", last_effect_link = "-1", last_active_joints = "-1";
    if (!init){

        for (int i=0;i<3;i++){
            last_joint_pos_DEG[i] = disp_curr_joint_pos_DEG[i];
            last_joint_vel[i] = disp_curr_joint_vel[i];
            last_joint_acc[i] = disp_curr_joint_acc[i];
            last_joint_torq[i] = disp_curr_joint_torq[i];
            last_des_carthesian_pos[i] = disp_des_carthesian_pos[i];
            last_carthesian_pos[i] = disp_curr_carthesian_pos[i];
        }
        for (int i =0;i<7;i++) last_rob_status[i] = disp_curr_rob_status[i];
        last_gripper_state = disp_gripper_state;
        last_light_barrier = disp_light_barrier;
        last_push_button = disp_push_button;
        last_err_code = disp_err_code;
        last_rob_model = disp_rob_model;
        last_refer_frame = disp_refer_frame;
        last_effect_link = disp_effect_link;
        last_active_joints = disp_active_joints;
        //Display everything in GUI
        ui->status_joint1pos_rad_3->display(disp_curr_joint_pos_DEG[0]*DEG_TO_RAD);
        ui->status_joint2pos_rad_3->display(disp_curr_joint_pos_DEG[1]*DEG_TO_RAD);
        ui->status_joint3pos_rad_3->display(disp_curr_joint_pos_DEG[2]*100.0);
        ui->status_joint1pos_deg_3->display(disp_curr_joint_pos_DEG[0]);
        ui->status_joint2pos_deg_3->display(disp_curr_joint_pos_DEG[1]);
        ui->status_joint3pos_deg_3->display(disp_curr_joint_pos_DEG[2]*100.0);
        ui->status_joint1vel_3->display(disp_curr_joint_vel[0]);
        ui->status_joint2vel_3->display(disp_curr_joint_vel[1]);
        ui->status_joint3vel_3->display(disp_curr_joint_vel[2]);
        ui->status_joint1acc_3->display(disp_curr_joint_acc[0]);
        ui->status_joint2acc_3->display(disp_curr_joint_acc[1]);
        ui->status_joint3acc_3->display(disp_curr_joint_acc[2]);
        ui->status_joint1torq_3->display(disp_curr_joint_torq[0]);
        ui->status_joint2torq_3->display(disp_curr_joint_torq[1]);
        ui->status_joint3torq_3->display(0);    //No torque response in Z-axis
        ui->status_pose_Xdesired->display(disp_des_carthesian_pos[0]);
        ui->status_pose_Ydesired->display(disp_des_carthesian_pos[1]);
        ui->status_pose_Zdesired->display(disp_des_carthesian_pos[2]);
        ui->status_pose_X->display(disp_curr_carthesian_pos[0]);
        ui->status_pose_Y->display(disp_curr_carthesian_pos[1]);
        ui->status_pose_Z->display(disp_curr_carthesian_pos[2]);
        ui->basicInfo_X_LCDnum_3->display(disp_curr_rob_status[0]); //x
        ui->basicInfo_Y_LCDnum_6->display(disp_curr_rob_status[1]); //y
        ui->basicInfo_Z_LCDnum_3->display(disp_curr_rob_status[2]); //z
        ui->basicInfo_R_LCDnum_3->display(disp_curr_rob_status[3]); //Rx
        ui->basicInfo_P_LCDnum_3->display(disp_curr_rob_status[4]); //Ry
        ui->basicInfo_Y_LCDnum_5->display(disp_curr_rob_status[5]); //Rz
        ui->basicInfo_W_LCDnum_3->display(disp_curr_rob_status[6]); //Rw
        ui->status_gripper_PushButton_3->display(disp_push_button);
        ui->status_gripper_LightBarrier_3->display(disp_light_barrier);
        ui->status_gripper_OnOff_3->display(disp_gripper_state);
        ROS_WARN("Started publishing text to GUI");
        ui->basicInfo_RobotModel_TextBrowser_3->setText(QString::fromStdString(disp_rob_model));
        ui->basicInfo_ReferenceFrame_TextBrowser_3->setText(QString::fromStdString(disp_refer_frame));
        ui->basicInfo_EffectorLink_TextBrowser_3->setText(QString::fromStdString(disp_effect_link));
        ui->basicInfo_ActiveJoints_TextBrowser_3->setText(QString::fromStdString(disp_active_joints));
        ROS_WARN("Publishing text to GUI finished");
        init = true;
    } else{

        if (last_joint_pos_DEG[0] != disp_curr_joint_pos_DEG[0]){           //Show **JOINT POSITIONS** in GUI
            ui->status_joint1pos_rad_3->display(disp_curr_joint_pos_DEG[0]*DEG_TO_RAD);
            ui->status_joint1pos_deg_3->display(disp_curr_joint_pos_DEG[0]);
            last_joint_pos_DEG[0] = disp_curr_joint_pos_DEG[0];
        }
        if (last_joint_pos_DEG[1] != disp_curr_joint_pos_DEG[1]){
            ui->status_joint2pos_rad_3->display(disp_curr_joint_pos_DEG[1]*DEG_TO_RAD);
            ui->status_joint2pos_deg_3->display(disp_curr_joint_pos_DEG[1]);
            last_joint_pos_DEG[1] = disp_curr_joint_pos_DEG[1];
        }
        if (last_joint_pos_DEG[2] != disp_curr_joint_pos_DEG[2]){
            ui->status_joint3pos_rad_3->display(disp_curr_joint_pos_DEG[2]*100.0);
            ui->status_joint3pos_deg_3->display(disp_curr_joint_pos_DEG[2]*100.0);
            last_joint_pos_DEG[2] = disp_curr_joint_pos_DEG[2];
        }
        if (last_joint_vel[0] != disp_curr_joint_vel[0]){                     //Show **JOINT VELOCITIES** in GUI
            //Write to GUI
            last_joint_vel[0] = disp_curr_joint_vel[0];
        }
        if (last_joint_vel[1] != disp_curr_joint_vel[1]){
            //Write to GUI
            last_joint_vel[1] = disp_curr_joint_vel[1];
        }
        if (last_joint_vel[2] != disp_curr_joint_vel[2]){
            //Write to GUI
            last_joint_vel[2] = disp_curr_joint_vel[2];
        }
        if (last_joint_acc[0] != disp_curr_joint_acc[0]){                     //Show **JOINT ACCELERATIONS** in GUI
            ui->status_joint1acc_3->display(disp_curr_joint_acc[0]);
            last_joint_acc[0] = disp_curr_joint_acc[0];
        }
        if (last_joint_acc[1] != disp_curr_joint_acc[1]){
            ui->status_joint2acc_3->display(disp_curr_joint_acc[1]);
            last_joint_acc[1] = disp_curr_joint_acc[1];
        }
        if (last_joint_acc[2] != disp_curr_joint_acc[2]){
            ui->status_joint3acc_3->display(disp_curr_joint_acc[2]);
            last_joint_acc[2] = disp_curr_joint_acc[2];
        }
        if (last_joint_torq[0] != disp_curr_joint_torq[0]){                   //Show **JOINT TORQUES** in GUI
            ui->status_joint1torq_3->display(disp_curr_joint_torq[0]);
            last_joint_torq[0] = disp_curr_joint_torq[0];
        }
        if (last_joint_torq[1] != disp_curr_joint_torq[1]){
            ui->status_joint2torq_3->display(disp_curr_joint_torq[1]);
            last_joint_torq[1] = disp_curr_joint_torq[1];
        }
        if (last_joint_torq[2] != disp_curr_joint_torq[2]){
            ui->status_joint3torq_3->display(disp_curr_joint_torq[2]);
            last_joint_torq[2] = disp_curr_joint_torq[2];
        }
        if (last_des_carthesian_pos[0] != disp_des_carthesian_pos[0]){        //Show **DESIRED CARTHESIAN POSITION** in GUI
            ui->status_pose_Xdesired->display(disp_des_carthesian_pos[0]);  //x
            last_des_carthesian_pos[0] = disp_des_carthesian_pos[0];
        }
        if (last_des_carthesian_pos[1] != disp_des_carthesian_pos[1]){      //y
            ui->status_pose_Ydesired->display(disp_des_carthesian_pos[1]);
            last_des_carthesian_pos[1] = disp_des_carthesian_pos[1];
        }
        if (last_des_carthesian_pos[2] != disp_des_carthesian_pos[2]){      //z
            ui->status_pose_Zdesired->display(disp_des_carthesian_pos[2]);
            last_des_carthesian_pos[2] = disp_des_carthesian_pos[2];
        }
        //Show **CARTHESIAN POSITION** in GUI
        if (last_carthesian_pos[0] != disp_curr_carthesian_pos[0]){      //x
            ui->status_pose_X->display(disp_curr_carthesian_pos[0]);
            last_carthesian_pos[0] = disp_curr_carthesian_pos[0];
        }
        if (last_carthesian_pos[1] != disp_curr_carthesian_pos[1]){      //y
            ui->status_pose_Y->display(disp_curr_carthesian_pos[1]);
            last_carthesian_pos[1] = disp_curr_carthesian_pos[1];
        }
        if (last_carthesian_pos[2] != disp_curr_carthesian_pos[2]){      //z
            ui->status_pose_Z->display(disp_curr_carthesian_pos[2]);
            last_carthesian_pos[2] = disp_curr_carthesian_pos[2];
        }
        //Show **ROBOT STATUS** in GUI
        if (last_rob_status[0] != disp_curr_rob_status[0]){      //x
            ui->basicInfo_X_LCDnum_3->display(disp_curr_rob_status[0]); //x
            last_rob_status[0] = disp_curr_rob_status[0];
        }
        if (last_rob_status[1] != disp_curr_rob_status[1]){      //y
            ui->basicInfo_Y_LCDnum_6->display(disp_curr_rob_status[1]); //y
            last_rob_status[1] = disp_curr_rob_status[1];
        }
        if (last_rob_status[2] != disp_curr_rob_status[2]){      //z
            ui->basicInfo_Z_LCDnum_3->display(disp_curr_rob_status[2]); //z
            last_rob_status[2] = disp_curr_rob_status[2];
        }
        if (last_rob_status[3] != disp_curr_rob_status[3]){      //RX
            ui->basicInfo_R_LCDnum_3->display(disp_curr_rob_status[3]); //Rx
            last_rob_status[3] = disp_curr_rob_status[3];
        }
        if (last_rob_status[4] != disp_curr_rob_status[4]){      //RY
            ui->basicInfo_P_LCDnum_3->display(disp_curr_rob_status[4]); //Ry
            last_rob_status[4] = disp_curr_rob_status[4];
        }
        if (last_rob_status[5] != disp_curr_rob_status[5]){      //RZ
            ui->basicInfo_Y_LCDnum_5->display(disp_curr_rob_status[5]); //Rz
            last_rob_status[5] = disp_curr_rob_status[5];
        }
        if (last_rob_status[6] != disp_curr_rob_status[6]){      //RW
            ui->basicInfo_W_LCDnum_3->display(disp_curr_rob_status[6]); //Rw
            last_rob_status[6] = disp_curr_rob_status[6];
        }
        //Show **GRIPPER STATE** in GUI
        if (last_gripper_state != disp_gripper_state){
            ui->status_gripper_OnOff_3->display(disp_gripper_state);
            last_gripper_state = disp_gripper_state;
        }
        //Show **LIGHT BARRIER** in GUI
        if (last_light_barrier != disp_light_barrier){
            ui->status_gripper_LightBarrier_3->display(disp_light_barrier);
            last_light_barrier = disp_light_barrier;
        }
        //Show **PUSH BUTTON** in GUI
        if (last_push_button != disp_push_button){
            ui->status_gripper_PushButton_3->display(disp_push_button);
            last_push_button = disp_push_button;
        }
        //Show **ERROR** in GUI
        if (last_err_code != disp_err_code){
            decodeErrorMessage(disp_err_code);
            last_err_code = disp_err_code;
        }
        //Show **ROBOT PARAMETERS** in GUI
        if (last_rob_model != disp_rob_model){
            ui->basicInfo_RobotModel_TextBrowser_3->setText(QString::fromStdString(disp_rob_model));
            last_rob_model = disp_rob_model;
        }
        if (last_refer_frame != disp_refer_frame){
            ui->basicInfo_ReferenceFrame_TextBrowser_3->setText(QString::fromStdString(disp_refer_frame));
            last_refer_frame = disp_refer_frame;
        }
        if (last_effect_link != disp_effect_link){
            ui->basicInfo_EffectorLink_TextBrowser_3->setText(QString::fromStdString(disp_effect_link));
            last_effect_link = disp_effect_link;
        }
        if (last_active_joints != disp_active_joints){
            ui->basicInfo_ActiveJoints_TextBrowser_3->setText(QString::fromStdString(disp_active_joints));
            last_active_joints = disp_active_joints;
        }
    }

}


void MainWindow::init_shared_variables(){
    disp_curr_joint_pos_DEG.reserve(3);
    disp_curr_joint_vel.reserve(3);
    disp_curr_joint_acc.reserve(3);
    disp_curr_joint_torq.reserve(3);
    disp_des_carthesian_pos.reserve(3);
    disp_curr_carthesian_pos.reserve(3);
    disp_curr_rob_status.reserve(7);
    for (int i=0;i<3;i++){
        disp_curr_joint_pos_DEG.push_back(0);
        disp_curr_joint_vel.push_back(0);
        disp_curr_joint_acc.push_back(0);
        disp_curr_joint_torq.push_back(0);
        disp_des_carthesian_pos.push_back(0);
        disp_curr_carthesian_pos.push_back(0);
    }
    for (int i=0;i<7;i++) disp_curr_rob_status.push_back(0);
    disp_gripper_state = 0;
    disp_light_barrier = 0;
    disp_push_button = 0;
    disp_err_code = -1;
    disp_rob_model = "0";
    disp_refer_frame = "0";
    disp_effect_link = "0";
    disp_active_joints = "0";
}


void MainWindow::decodeErrorMessage(uint8_t error_message){
    switch (error_message){
        case 0:
            ui->error_lineEdit->setText("Everything OK!");
            break;
        case 1:
            ui->error_lineEdit->setText("[joint control] : Bad input joint values");
            break;
        case 2:
            ui->error_lineEdit->setText("[joint control] : Bad plan");
            break;
        case 3:
            ui->error_lineEdit->setText("[position control] : Colision warining! changing mode");
            break;
        case 4:
            ui->error_lineEdit->setText("[position control] : Cannot solve IK please enter new positions");
            break;
        case 5:
            ui->error_lineEdit->setText("[position control] : No solution found for desired position");
            break;
        case 6:
            ui->error_lineEdit->setText("[joint control] : Something wrong with the function and cannot execute plan");
            break;
        case 7:
            ui->error_lineEdit->setText("Bad plan or out of bounds ! cannot execute plan");
            break;
        case 8:
            ui->error_lineEdit->setText("[DEMO control] : replanning trajectory");
            break;
        case 9:
            ui->error_lineEdit->setText("[TEACH mode GUI] : teached new position!");
            break;
        case 10:
            ui->error_lineEdit->setText("[TEACH mode GUI] : stopped teaching!");
        case 11:
            ui->error_lineEdit->setText("[TEACH mode GUI] : size NOT OK!");
            break;
        case 12:
            ui->error_lineEdit->setText("[TEACH mode HAND] : teached new position!");
            break;
        case 13:
            ui->error_lineEdit->setText("[TEACH mode HAND] : stopped teaching!");
            break;
        case 14:
            ui->error_lineEdit->setText("[TEACH mode HAND] : size NOT OK!");
            break;
        case 15:
            ui->error_lineEdit->setText("[TEACH mode HAND] : size NOT OK!");
            break;
        default:
            break;
    }
}