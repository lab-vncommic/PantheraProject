
#include <Stdlib.h>
#include <String.h>
#include <math.h>
#include <Psim.h>

// PLACE GLOBAL VARIABLES OR USER FUNCTIONS HERE...
#include "adc.h"
#include "adc.c"
#include "gpio.h"
#include "gpio.c"

#include "GlobalDefine.h"
#include "GlobalDefine.c"
#include "Button.h"
#include "Button.c"
#include "IQMath.h"
#include "IQMath.c"
#include "Measurement.h"
#include "Measurement.c"
#include "FaultManagement.h"
#include "FaultManagement.c"
#include "MotorDriver.h"
#include "MotorDriver.c"
#include "StateManagement.h"
#include "StateManagement.c"

#include "Panthera.h"
#include "Panthera.c"

/////////////////////////////////////////////////////////////////////
// FUNCTION: SimulationStep
//   This function runs at every time step.
//double t: (read only) time
//double delt: (read only) time step as in Simulation control
//double *in: (read only) zero based array of input values. in[0] is the first node, in[1] second input...
//double *out: (write only) zero based array of output values. out[0] is the first node, out[1] second output...
//int *pnError: (write only)  assign  *pnError = 1;  if there is an error and set the error message in szErrorMsg
//    strcpy(szErrorMsg, "Error message here..."); 
// DO NOT CHANGE THE NAME OR PARAMETERS OF THIS FUNCTION
void SimulationStep(
		double t, double delt, double *in, double *out,
		 int *pnError, char * szErrorMsg,
		 void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
{
// ENTER YOUR CODE HERE...
// Update input value
thisGPIO.height_1.value 	   = in[0];
thisGPIO.height_2.value 	   = in[1];
thisGPIO.save.value 	  	   = in[2];
thisGPIO.recall_1.value 	   = in[3];
thisGPIO.recall_2.value 	   = in[4];
thisGPIO.recall_3.value 	   = in[5];
thatADC.ADC_MulSens_1 	  	   = in[6];
motor_height.driver.hall.value = in[7];

PTR_main_interrupt_service();

out[0] 	= motor_height.driver.ina.value;     
out[1] 	= motor_height.driver.inb.value;          // Example output, replace with your logic
out[2] 	= motor_height.driver.pwm.value;
out[3] 	= motor_height.driver.sel0.value;
out[4] 	= motor_height.driver.sel1.value;
out[5] 	= motor_height.driver.mul_en.value;
out[6] 	= thisPTR.state;
out[7] 	= thisButton.btn_stt;
out[8] 	= thisButton.name;;
out[9] 	= thisButton.cnt_action;
out[10] = thisButton.action;
out[11] = counter;
out[12] = thisButtonAll.height_1.value;
}


/////////////////////////////////////////////////////////////////////
// FUNCTION: SimulationBegin
//   Initialization function. This function runs once at the beginning of simulation
//   For parameter sweep or AC sweep simulation, this function runs at the beginning of each simulation cycle.
//   Use this function to initialize static or global variables.
//const char *szId: (read only) Name of the C-block 
//int nInputCount: (read only) Number of input nodes
//int nOutputCount: (read only) Number of output nodes
//int nParameterCount: (read only) Number of parameters is always zero for C-Blocks.  Ignore nParameterCount and pszParameters
//int *pnError: (write only)  assign  *pnError = 1;  if there is an error and set the error message in szErrorMsg
//    strcpy(szErrorMsg, "Error message here..."); 
// DO NOT CHANGE THE NAME OR PARAMETERS OF THIS FUNCTION
void SimulationBegin(
		const char *szId, int nInputCount, int nOutputCount,
		 int nParameterCount, const char ** pszParameters,
		 int *pnError, char * szErrorMsg,
		 void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
{
// ENTER INITIALIZATION CODE HERE...
PTR_system_initialization();



}


/////////////////////////////////////////////////////////////////////
// FUNCTION: SimulationEnd
//   Termination function. This function runs once at the end of simulation
//   For parameter sweep or AC sweep simulation, this function runs at the end of each simulation cycle.
//   Use this function to de-allocate any allocated memory or to save the result of simulation in an alternate file.
// Ignore all parameters for C-block 
// DO NOT CHANGE THE NAME OR PARAMETERS OF THIS FUNCTION
void SimulationEnd(const char *szId, void ** reserved_UserData, int reserved_ThreadIndex, void * reserved_AppPtr)
{




}


