
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
thisButton.height_1.value = in[0];
thisButton.height_2.value = in[1];
thisButton.save.value 	  = in[2];
thisButton.recall_1.value = in[3];
thisButton.recall_2.value = in[4];
thisButton.recall_3.value = in[5];
thatADC.ADC_MulSens_1 	  = in[6];

PTR_main_interrupt_service();

out[0] = thisGPIO.INA.value;     
out[1] = thisGPIO.INB.value;          // Example output, replace with your logic
out[2] = thisGPIO.PWM.value;
out[3] = thisGPIO.SEL0.value;
out[4] = thisGPIO.SEL1.value;
out[5] = thisGPIO.MulEN.value;
out[6] = thisFault.fault_status;
out[7] = thisMeas.current_mt1.raw;
out[8] = thisMeas.current_mt1.value_f32;
out[9] = thisMeas.current_mt1.value_q15;
out[10] = thatADC.ADC_MulSens_1;
out[11] = thisMeas.current_mt1.value_q15;
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


