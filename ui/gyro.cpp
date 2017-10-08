#include "gyro.h"

//DigitalOut myled(LED1);
//Serial pc(USBTX, USBRX);    // tx, rx for USB debug printf to terminal console
I2C i2c(p9, p10);          // LPC1768 I2C pin allocation
//DigitalIn din(p23); // used as a test button

// Globals
int16_t const   Offset_mX=-40.0;
int16_t const   Offset_mY=-115.0;
float const     RadtoDeg=(180.0/3.141592654);

MLX90614 IR_thermometer(&i2c);

float get_temperature(void){
    float temp;
    IR_thermometer.getTemp(&temp);
    return temp;
}

char readByte(char address, char reg)
// Reads one byte from an I2C address
// Didn't bother to make a multi-byte version to read in X,Y,Z low/high series of registers because...
// the data registers of all sensors they are in the same XL,XH,YL,YH,ZL,ZH order apart from the magnetometer which is XH,XL,ZH,ZL,YH,YL...
{
    char result;
    
    i2c.start();
    i2c.write(address);         // Slave address with direction=write
    i2c.write(reg);             // Subaddress (register)
    
    i2c.start();                // Break transmission to change bus direction
    i2c.write(address + 1);     // Slave address with direction=read [bit0=1]
    
    result = i2c.read(0);
    i2c.stop();
    return (result);
    }
    
void writeByte(char address, char reg,char value)
// Sends 1 byte to an I2C address
{    
    i2c.start();
    i2c.write(address);         // Slave address
    i2c.write(reg);             // Subaddress (register)
    i2c.write(value);
    i2c.stop();
    
    }
    
void initSensors (void)
// Switch on and set up the 3 on-board sensors
{
    //pc.printf("--------------------------------------\n");
    //pc.printf("\nSTM MEMS eval board sensor init \n");
    
#ifdef LSM303_on
    // LSM303DLHC Magnetic sensor
    //pc.printf("LSM303DLHC ping (should reply 0x48): %x \n",readByte(LSM303_m,mIRA_REG_M));
    writeByte(LSM303_m,mCRA_REG_M,0x94);     //switch on temperature sensor and set Output Data Rate to 30Hz
    writeByte(LSM303_m,mCRB_REG_M,0x20);     // Set the gain for +/- 1.3 Gauss full scale range
    writeByte(LSM303_m,mMR_REG_M,0x0);       // Continuous convertion mode
    
    // LSM303DLHC Accelerometer
    writeByte(LSM303_a,aCTRL_REG1_A ,0x37); // Set 25Hz ODR, everything else on
    writeByte(LSM303_a,aCTRL_REG4_A ,0x08); // Set full scale to +/- 2g sensitivity and high rez mode
#endif
    
    //pc.printf("--------------------------------------\n \n");
    wait(2); // Wait for settings to stabilize
    }    

void LSM303 (SensorState_t * state)
// Magnetometer and accelerometer
{
    char        xL, xH, yL, yH, zL, zH;
    int16_t     mX, mY, mZ,aX,aY,aZ;
    float       pitch,roll,faX,faY;

    xL=readByte(LSM303_m,mOUT_X_L_M);
    xH=readByte(LSM303_m,mOUT_X_H_M);
    yL=readByte(LSM303_m,mOUT_Y_L_M);
    yH=readByte(LSM303_m,mOUT_Y_H_M);
    zL=readByte(LSM303_m,mOUT_Z_L_M);
    zH=readByte(LSM303_m,mOUT_Z_H_M);
    
    mX=(xH<<8) | (xL); // 16-bit 2's complement data
    mY=(yH<<8) | (yL);
    mZ=(zH<<8) | (zL);
    
    //pc.printf("mX=%hd   %X          mY=%hd  %X      mZ=%hd  %X      \n",mX,mX,mY,mY,mZ,mZ);
    
    mX=mX-Offset_mX; // These are callibration co-efficients to deal with non-zero soft iron magnetic offset
    mY=mY-Offset_mY; 
    
    xL=readByte(LSM303_a,aOUT_X_L_A);
    xH=readByte(LSM303_a,aOUT_X_H_A);
    yL=readByte(LSM303_a,aOUT_Y_L_A);
    yH=readByte(LSM303_a,aOUT_Y_H_A);
    zL=readByte(LSM303_a,aOUT_Z_L_A);
    zH=readByte(LSM303_a,aOUT_Z_H_A);

    aX=(signed short) ( (xH<<8) | (xL) ) >> 4; // 12-bit data from ADC. Cast ensures that the 2's complement sign is not lost in the right shift.
    aY=(signed short) ( (yH<<8) | (yL) ) >> 4;
    aZ=(signed short) ( (zH<<8) | (zL) ) >> 4;
    
    //pc.printf("aX=%hd   %X          aY=%hd  %X      aZ=%hd  %X      \n",aX,aX,aY,aY,aZ,aZ);
    
    faX=((float) aX) /2000.0; // Accelerometer scale I chose is 1mg per LSB with range +/-2g. So to normalize for full scale need to divide by 2000.
    faY=((float) aY) /2000.0; // If you don't do this the pitch and roll calcs will not work (inverse cosine of a value greater than 1)
    //faZ=((float) aZ) /2000.0; // Not used in a calc so comment out to avoid the compiler warning
        
    // Trigonometry derived from STM app note AN3192 and from WikiRobots
   pitch = asin((float) -faX*2); // Dividing faX and faY by 1000 rather than 2000 seems to give better tilt immunity. Do it here rather than above to preserve true mg units of faX etc
   roll = asin(faY*2/cos(pitch));
   
  float xh = mX * cos(pitch) + mZ * sin(pitch);
  float yh = mX * sin(roll) * sin(pitch) + mY * cos(roll) - mZ * sin(roll) * cos(pitch);
  float zh = -mX * cos(roll) * sin(pitch) + mY * sin(roll) + mZ * cos(roll) * cos(pitch);
 
  float heading = atan2(yh, xh) * RadtoDeg; // Note use of atan2 rather than atan since better for working with quadrants
  if (yh < 0)
    heading=360+heading;
    
    state->heading=heading;
    state->pitch=pitch;
    state->roll=roll;
    state->aX = (float)aX;
    state->aY = (float)aY;
    state->aZ = (float)aZ;
    //5.1f 
    //pc.printf("Orientation (deg):   Rot_X: %0.0f    Rot_Y: %0.0f     Rot_Z: %0.0f \n",roll*RadtoDeg,pitch*RadtoDeg,heading);
    //pc.printf("Acceleration (mg):   X: %5hd         Y: %5hd          Z: %5hd \n",aX,aY,aZ);    
    
}

//calculates the difference for acceleration in int16_t value  
void calc_avrg_ac(Result_avrg* result,int samples){
    int i = 0;
    result -> aX = 0;
    result -> aY = 0;
    result -> aZ = 0;
    SensorState_t   state;
    for(i = 0;i<samples;i++){
        #ifdef LSM303_on
            LSM303(&state);
        #endif
        result -> aX += state.aX;
        result -> aY += state.aY;
        result -> aZ += state.aZ;
        //pc.printf("Acceleration (mg):   X: %5hd         Y: %5hd          Z: %5hd \n",state.aX,state.aY,state.aZ);
           
        wait(0.01);
    }
    //pc.printf("Acceleration (mg):   X: %5hd         Y: %5hd          Z: %5hd \n",result->aX,result->aY,result->aZ);   
    result -> aX = result -> aX / samples;
    result -> aY = result -> aY / samples;
    result -> aZ = result -> aZ / samples;
    //pc.printf("rAcceleration (mg):   X: %5hd         Y: %5hd          Z: %5hd \n",result->aX,result->aY,result->aZ);   
}

//calculates the difference for orientation in float value
void calc_avrg_or(Result_avrg* result,int samples){
    int i = 0;
    result -> x = 0;
    result -> y = 0;
    result -> z = 0;
    SensorState_t   state;
    for(i = 0;i<samples;i++){
        #ifdef LSM303_on
            LSM303(&state);
        #endif
        result -> x +=  state.roll*RadtoDeg;
        result -> y += state.pitch*RadtoDeg;
        result -> z += state.heading;
        //pc.printf("Orientation (deg):   Rot_X: %0.0f    Rot_Y: %0.0f     Rot_Z: %0.0f \n",state.roll*RadtoDeg,state.pitch*RadtoDeg,state.heading);
        wait(0.01);
    }
    //pc.printf("Orientation (deg):   Rot_X: %0.0f    Rot_Y: %0.0f     Rot_Z: %0.0f \n", result -> x ,result -> y,result -> z);
    result -> x = result -> x / samples;
    result -> y = result -> y / samples;
    result -> z = result -> z / samples;
    //pc.printf("Orientation (deg):   rRot_X: %0.0f    rRot_Y: %0.0f     rRot_Z: %0.0f \n", result -> x ,result -> y,result -> z);
}

//gets the two results and saves the answer in r1 structure
void calc_diff(Result_avrg* r1, Result_avrg* r2){
    r1 -> x = abs(r1->x - r2->x);
    r1 -> y = abs(r1->y - r2->y);
    r1 -> z = abs(r1->z - r2->z);
    
    r1 -> aX = abs(r1->aX - r2->aX);
    r1 -> aY = abs(r1->aY - r2->aY);
    r1 -> aZ = abs(r1->aZ - r2->aZ);
}

/*

int main()
{
    SensorState_t   state;
    Result_avrg result1;
    //Result_avrg result2;
    initSensors();
    //pc.baud(115200);
    calc_avrg_or(&result1,3);
    //calc_avrg_ac(&result1,3);
    //pc.printf("Orientation (deg):   Rot_X: %0.0f    Rot_Y: %0.0f     Rot_Z: %0.0f \n", result1.x ,result1.y,result1.z);
    //pc.printf("Acceleration (mg):   X: %5hd         Y: %5hd          Z: %5hd \n",result1.aX,result1.aY,result1.aZ);   
    //calc_avrg_or(&result2,3);
    //calc_avrg_ac(&result2,3);
    //pc.printf("Orientation (deg):   Rot_X: %0.0f    Rot_Y: %0.0f     Rot_Z: %0.0f \n", result2.x ,result2.y,result2.z);
    //pc.printf("Acceleration (mg):   X: %5hd         Y: %5hd          Z: %5hd \n",result2.aX,result2.aY,result2.aZ);
    //calc_diff(&result1,&result2);
    //pc.printf("Orientation (deg):   Rot_X: %0.0f    Rot_Y: %0.0f     Rot_Z: %0.0f \n", result1.x ,result1.y,result1.z);
    //pc.printf("Acceleration (mg):   X: %5hd         Y: %5hd          Z: %5hd \n",result1.aX,result1.aY,result1.aZ);
#ifdef LSM303_on
    //LSM303(&state);
#endif

    //pc.printf("Orientation (deg):   Rot_X: %0.0f    Rot_Y: %0.0f     Rot_Z: %0.0f \n",state.roll*RadtoDeg,state.pitch*RadtoDeg,state.heading);
    //pc.printf("Acceleration (mg):   X: %5hd         Y: %5hd          Z: %5hd \n",state.aX,state.aY,state.aZ);  
    //pc.printf("\n");
    
}
*/