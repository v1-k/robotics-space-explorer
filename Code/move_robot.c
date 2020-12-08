
#include <ev3.h>
const float  MPI = 22/7;
int m_a ,m_b,m_ab, speed = 20;  	// Speed
float constant_a, constant_b, rotate_degree = 360, tire_dia_mm ,tire_width_mm, tires_distance_cm,tire_total_dis_cm;
float rotation_distance_a_m, rotation_distance_b_m, tire_circum_m, degree_multiplier_m;


int flag ,count,randNum ;
float total_distance_m ,distance_constant;
float initialRoatate,normalRotate,uturnRotate,two_third_rotate;
float moveBackDistance;
int goalDistance;

int wallColor,goalColor,minRange,rotationCounter;
void initData(){
	 m_a = OUT_A;  		// Motor A
	 m_b = OUT_B;  		// Motor B
	 m_ab = OUT_AB; 		// Motor A and B
	 speed = 18;  	// Speed
	 constant_a = -0.02068;
	 constant_b = -0.02165	;
	 rotate_degree = 360;							// motor rotation 360 degree , one cycle

	  tire_dia_mm = 56;  							// Tire Diameter in centimeter
	  tire_width_mm = 28;							// Tire width in millimeter
	  tires_distance_cm = 9.3;						// distance between tires from inside edge to inside edge in centimeter
	  tire_total_dis_cm = tires_distance_cm + (tire_width_mm/2+tire_width_mm/2)/10;	// distance between tires from center to center in centimeter
	  rotation_distance_a_m = MPI*(tire_total_dis_cm/(2*100))+constant_a;	// Rotation distance for turning 90 degree in meter
	  rotation_distance_b_m = MPI*(tire_total_dis_cm/(2*100))+constant_b;	// Rotation distance for turning 90 degree in meter
	  tire_circum_m = MPI*(tire_dia_mm/1000); 		// Calculate Tire Circumference in meters
	  degree_multiplier_m = tire_circum_m/rotate_degree;		// multiplier for 1 degree rotation distance travel by tire in meter


	 flag = 1;
	 count = 1;
	 total_distance_m = 0.305;

	 initialRoatate = 85+43;
	 normalRotate = 87;
	 uturnRotate = 177;
	 distance_constant = 0.08;//in meters
	 moveBackDistance = 0.06;
	 wallColor = 2;
	 goalColor = 5;
	 minRange = 1000;
	 rotationCounter = 0;
	 two_third_rotate = 266;
}
void moveBack(float distance){

	SetAllSensorMode(GYRO_RATE, US_DIST_MM, COL_COLOR, NO_SEN);
	Wait(500);
	SetAllSensorMode(GYRO_ANG, US_DIST_MM, COL_COLOR, NO_SEN);
	OnRevSync(m_ab,speed+10);

	while(ReadSensor(IN_3) == wallColor){
		TermPrintln("Color %d \n",ReadSensor(IN_3));
	}
	ResetRotationCount(m_a);
	ResetRotationCount(m_b);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
		TermPrintln("Color in while %d \n",ReadSensor(IN_3));

		//find wall
		if (degree_multiplier_m*abs(MotorRotationCount(m_a)) >= distance){
			Off(m_a);
			Off(m_b);
			break;
		}
	}
}
int CheckConstDistance(int motor,int motorO){
	ResetRotationCount(m_a);
	ResetRotationCount(m_b);
	OnFwdSync(m_ab,speed);
	SetAllSensorMode(GYRO_RATE, US_DIST_MM, COL_COLOR, NO_SEN);
	Wait(500);
	SetAllSensorMode(GYRO_ANG, US_DIST_MM, COL_COLOR, NO_SEN);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
			//find wall
			if ((degree_multiplier_m*abs(MotorRotationCount(m_a)) >= distance_constant) || (ReadSensor(IN_3) ==wallColor) ){
				Off(m_a);
				Off(m_b);
				if(ReadSensor(IN_3) ==wallColor){
					moveBack(moveBackDistance);
					return 2;
				}
				else{

					return 1;
				}

			}
	}
}
int rotateAndCheck(float rotateDegree,int motor,int motorO){
	Wait(500);
	ResetRotationCount(motor);
	ResetRotationCount(motorO);
	SetAllSensorMode(GYRO_RATE, US_DIST_MM, COL_COLOR, NO_SEN);
	Wait(500);
	SetAllSensorMode(GYRO_ANG, US_DIST_MM, COL_COLOR, NO_SEN);
	//TermPrintln("Color %d \n",ReadSensor(IN_3));
	OnFwdReg(motor,speed-10);
	OnRevReg(motorO,speed-10);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
		TermPrintln("Gyro  %d \n",ReadSensor(IN_1));
		TermPrintln("Sonar %d \n",ReadSensor(IN_2));
		TermPrintln("Color %d \n",ReadSensor(IN_3));

		if (abs(ReadSensor(IN_1)) >=rotateDegree ) {
			Off(motor);
			Off(motorO);
			return CheckConstDistance(motor,motorO);
		}
	}
}

int findWallLater(){
	ResetRotationCount(m_a);
	ResetRotationCount(m_b);
	OnFwdSync(m_ab,speed);
	SetAllSensorMode(GYRO_RATE, US_DIST_MM, COL_COLOR, NO_SEN);
	Wait(500);
	SetAllSensorMode(GYRO_ANG, US_DIST_MM, COL_COLOR, NO_SEN);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
			//find goal
			if(ReadSensor(IN_3) ==goalColor){
				Off(m_a);
				Off(m_b);
//				findGoal();
				return 2;
			}
			//find wall
			if(ReadSensor(IN_3) ==wallColor){
				Off(m_a);
				Off(m_b);
				moveBack(moveBackDistance);
//				rotateAndCheck(normalRotate,motor,motorO);
				return 1;
			}
	}
}
void rotate(float rotateDegree,int motor,int motorO){
	Wait(500);
	ResetRotationCount(motor);
	ResetRotationCount(motorO);
	SetAllSensorMode(GYRO_RATE, US_DIST_MM, COL_COLOR, NO_SEN);
	Wait(500);
	SetAllSensorMode(GYRO_ANG, US_DIST_MM, COL_COLOR, NO_SEN);
	//TermPrintln("Color %d \n",ReadSensor(IN_3));
	OnFwdReg(motor,speed-10);
	OnRevReg(motorO,speed-10);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
		TermPrintln("Gyro  %d \n",ReadSensor(IN_1));
		TermPrintln("Sonar %d \n",ReadSensor(IN_2));
		TermPrintln("Color %d \n",ReadSensor(IN_3));

		if (abs(ReadSensor(IN_1)) >=rotateDegree ) {
			Off(motor);
			Off(motorO);
			break;
		}
	}
}
void playSound(){
	//PlayTone(TONE_C2,NOTE_EIGHT);
	PlaySound(SOUND_DOUBLE_BEEP);\
	Wait(1000);
	StopSound();
}
int rotateAndFind(float rotateDegree,int motor,int motorO){

	ResetRotationCount(motor);
	ResetRotationCount(motorO);
	goalDistance = 2550;
	SetAllSensorMode(GYRO_RATE, US_DIST_MM, COL_COLOR, NO_SEN);
	Wait(500);
	SetAllSensorMode(GYRO_ANG, US_DIST_MM, COL_COLOR, NO_SEN);
	//TermPrintln("Color %d \n",ReadSensor(IN_3));
	OnFwdReg(motor,speed-10);
	OnRevReg(motorO,speed-10);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
		TermPrintln("Gyro  %d \n",ReadSensor(IN_1));
		TermPrintln("Sonar %d \n",ReadSensor(IN_2));
		TermPrintln("Color %d \n",ReadSensor(IN_3));
		if (abs(ReadSensor(IN_2))  < goalDistance  && abs(ReadSensor(IN_2)) < minRange){
			goalDistance = ReadSensor(IN_2);
		}
		if (abs(ReadSensor(IN_1)) >=rotateDegree ) {
			Off(motor);
			Off(motorO);
			return goalDistance;
		}
	}
}
void rotateAndPoint(int goal_distance,int motor,int motorO){

	ResetRotationCount(motor);
	ResetRotationCount(motorO);
	SetAllSensorMode(GYRO_RATE, NO_SEN, COL_COLOR, NO_SEN);
	Wait(500);
	SetAllSensorMode(GYRO_ANG, US_DIST_MM, COL_COLOR, NO_SEN);
	OnFwdReg(motorO,speed-10);
	OnRevReg(motor,speed-10);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
		TermPrintln("goal_distance %d \n",goal_distance);
		TermPrintln("sensor data %d \n",abs(ReadSensor(IN_2)));

		//245 < 255 < 255
		if (//goal_distance-5 < abs(ReadSensor(IN_2)) &&
				abs(ReadSensor(IN_2))  < goal_distance+5 && abs(ReadSensor(IN_2)) < minRange){
			Off(motor);
			Off(motorO);
			break;
		}
	}
}
void clearObject(){
	ResetRotationCount(m_a);
	ResetRotationCount(m_b);
	OnFwdSync(m_ab,speed);

	while(!ButtonIsDown(BUTTON_ID_ENTER)){
		if (degree_multiplier_m*MotorRotationCount(m_a) >= total_distance_m*1.5) {
				Off(m_a);
				Off(m_b);
				break;
		}
	}
	moveBack(total_distance_m*1.3);
}
void goalClearing(){
	moveBack(moveBackDistance);
	int goal_distance = rotateAndFind( 340, m_a,m_b);
	rotateAndPoint( goal_distance, m_a,m_b);
	clearObject();
}

void findGoal(){
	playSound();
	goalClearing();
}
int findWall(){
	ResetRotationCount(m_a);
	ResetRotationCount(m_b);
	OnFwdSync(m_ab,speed);
	SetAllSensorMode(GYRO_RATE, US_DIST_MM, COL_COLOR, NO_SEN);
	Wait(500);
	SetAllSensorMode(GYRO_ANG, US_DIST_MM, COL_COLOR, NO_SEN);
	while(!ButtonIsDown(BUTTON_ID_ENTER)){
			//find goal
			if(ReadSensor(IN_3) ==goalColor){
				Off(m_a);
				Off(m_b);

				return 2;
			}
			//find wall
			if(ReadSensor(IN_3) ==wallColor){
				Off(m_a);
				Off(m_b);
				moveBack(moveBackDistance);
				return 1;
			}
	}
}
int getRandom(){
	int nMax = 2;
	int nMin = 1;
	srand(time(NULL));
	return  rand()%((nMax+1)-nMin) + nMin;
}
int main(void)
{

	InitEV3();
	initData();
	float rotateDegreeTemp;
	if (findWall() == 2){
		findGoal();
	}
	else{
		//rotationCounter = rotationCounter+1;
		rotate(initialRoatate,m_a,m_b);
		while(!ButtonIsDown(BUTTON_ID_ENTER)){
			if (findWallLater()==2){
				findGoal();
				break;
			}
			else{

				int rndy = getRandom();
				switch(rndy) {
				      case 1 :
				    	  rotateDegreeTemp = 44;
				    	  break;
				      case 2 :
				    	  rotateDegreeTemp = normalRotate;
						  break;
					  default:
						  rotateDegreeTemp = normalRotate;
						  break;
				   }
				if (rotateAndCheck(rotateDegreeTemp,m_a,m_b)==2){
					rotate(uturnRotate,m_a,m_b);
				}
			}
		}
	}
	FreeEV3();

	return 0;
}
