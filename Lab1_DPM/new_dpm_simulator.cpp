/*
*  main.cpp
*  DPM_simulator
*
*  Created by Donghwa Shin on 10/19/13.
*  Modified by Sara Vinco on 10/20/14.
*/

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#define MAXPATH 256
#define MAXSTATENAME 50
#define HIST_WIND_SIZE 1
#define TIME_UNIT   0.000001    //in us
#define POWER_UNIT  0.001       // in mW
#define ENERGY_UNIT  0.000001   // in uJ
#define MAX_EMUL_TIME 500000
#define ACTIVE  0
#define IDLE    1
#define SLEEP   2
#define TIMEOUT          0
#define HISTORY_BASED    1
#define ADAPTIVE         2
// MV from here
#define POLYN 3 // how many coefficients  k1... , (NOTE k0 not included!!) appears in the expression: 1, 5, 9 for the 3 cases (with win>1)
// Following macro describe the type of equation used. Comment them out if not needed
#define WIN_1 0   // Polynomial equation with only 1 indipendent variable
//#define WIN_2 1 // Two indipendent variables
//#define WIN_3 2 // three indipendent variables
// MV end


/*
* Function:   print_command_line
* Parameters:
* Behavior:   prints command line
* Return:     false to prevend DPM execution
*/
void print_command_line(){
  printf("\n******************************************************************************\n");
  printf(" DPM simulator: \n");
  printf("\t-t <timeout>: timeout of the timeout policy\n");
  printf("\t-h <Value1> …<Value5> <Value6> <Value7>: history-based predictive policy \n");
  printf("\t   <Value1-5> value of coefficients\n");
  printf("\t   <Value6> threshold 1, <Value7> is threshold 2\n");
  printf("\t-psm <psm filename>: the power state machine (PSM, default is psm.txt)\n");
  printf("\t-wl <wl filename>: the workload (default is wl.txt)\n");
  printf("******************************************************************************\n\n");
}

/*
* Function:   parse_inputs
* Parameters: arg and argv from command line;
*             pointers to files storing workload (fwl) and power state machine (psm);
*             timeout for timeout policy (timeout);
*             list of parameters for history based policies (hist_alpha);
*             prediction thresholds for history based policies (prediction_threshold1 and prediction_threshold2);
*             adopted policy (policy).
* Behavior:   parses inputs to gather policy configuration and input files
* Return:     true if command line is correct, false otherwise
*/
bool parse_arg(int argc, char *argv[], char *fwl, char *fpsm, double &timeout,
               double *hist_alpha, double &prediction_threshold1, double &prediction_threshold2, int &policy, double &sleep_timeout)
{
	int cur = 1;
	while(cur < argc)
	{
		// set policy to timeout and get timeout value
		if(strcmp(argv[cur], "-help") == 0)
		{
		  print_command_line();
		  return false;
		}
		// set policy to timeout and get timeout value
		if(strcmp(argv[cur], "-t") == 0)
		{
			policy = TIMEOUT;
			if(argc > cur + 1) // && strcmp(argv[cur + 1], "-dpm") != 0 && strcmp(argv[cur + 1], "-wl") != 0)
			{
				timeout = atof(argv[cur + 1]);
				cur ++;
                // MV
                sleep_timeout = atof(argv[cur + 1]);
                if(sleep_timeout < timeout){
                    printf("error: sleep timeout must be greater than normal timeout!\n");
                    return false;
                }

                cur ++;
                // end MV
			}
			else
				return	false;

		}
		// set policy to history based and get parameters and thresholds
		if(strcmp(argv[cur], "-h") == 0)
		{
			policy = HISTORY_BASED;
			if(argc > cur + 1){
				int i;
				for(i=0;i<POLYN+1;i++){      // was HIST_WIND_SIZE // MV
				    hist_alpha[i] = atof(argv[cur + 1]);
            cur++;
				}
				prediction_threshold1 = atof(argv[cur + 1]);
				prediction_threshold2 = atof(argv[cur + 2]);
				cur = cur + POLYN + 3; // was + 2, +2 for the above two lines, +1 because POLYN does not include the last coefficient// MV
			}
			else
				return	false;
		}
		// set policy to adaptative
		if(strcmp(argv[cur], "-a") == 0)
		{
			policy = ADAPTIVE;
			/*
			Fill out here if you need
			*/

		}
		// set name of file for the power state machine
		if(strcmp(argv[cur], "-psm") == 0)
		{
			if(argc > cur + 1)
			{
				strcpy(fpsm, argv[cur + 1]);
				cur ++;
			}
			else
				return	false;
		}
		// set name of file for the workload
		if(strcmp(argv[cur], "-wl") == 0)
		{
			if(argc > cur + 1)
			{
				strcpy(fwl, argv[cur + 1]);
				cur ++;
			}
			else
				return	false;
		}
		cur ++;
	}
	return true;
}

/*
* Function:   init
* Parameters: pointers to files storing the power state machine (psm);
*             pointers to data structures and variables to be initialized
* Behavior:   allocates and initializes memory; and loads data about:
*             number and names of power states (numstate, psm_name)
*             power per state (ppow), transition costs in terms of energy (psm_energy),
*             transition cost in terms of time (psm_time)
* Return:     true if no error occurred, else otherwise
*/
inline bool init(char * fpsm, double *&psm_energy, double *&psm_time, char *&psm_name, double *&ppow, int &numstate)
{
	// open the power state machine file
	FILE *fp;
	int cnt;
	fp = fopen(fpsm, "r");
	if(!fp)
	{
		printf("error: can't open file %s!\n", fpsm);
		return false;
	}
	fscanf(fp, "%d", &numstate); //read number of states
	psm_name = (char *)calloc(numstate, sizeof(char));

	int i;

	//read the state names
	for(i = 0; i < numstate; i ++)
		if(!fscanf(fp, "%s", psm_name + i))
		{
			printf("error reading file %s!\n", fpsm);
			fclose(fp);
			return false;
		}

	// allocate memory
	cnt = numstate * numstate;
	ppow = (double *)calloc(numstate, sizeof(double));
	psm_energy = (double *)calloc(cnt, sizeof(double));
	psm_time = (double *)calloc(cnt, sizeof(double));

	//read the power for each state
	for(i = 0; i < numstate; i ++)
	{
		if(!fscanf(fp, "%lf", ppow + i))
		{
			printf("error reading file %s!\n", fpsm);
			fclose(fp);
			return false;
		}
	}

	//read the transition costs
	for(i = 0; i < cnt; i ++)
	{
		if(!fscanf(fp, "%lf/%lf", psm_energy + i, psm_time + i))
		{
			printf("error reading file %s!\n", fpsm);
			fclose(fp);
			return false;
		}
        printf("transition energy %d = %.0fuJ, time = %.0fus\n", i, psm_energy[i],psm_time[i]);
	}

	// close the power state machine file
	fclose(fp);

	for(i = 0; i < numstate; i ++){
		printf("P_state%d = %.0fmW\n", i,ppow[i]);
    }
    printf("\n");
    return true;
}

/*
* Function:   decide_state
* Parameters: characteristics of current simulation slot: current time (curr_time),
*             beginning and end of idle period (idle_period_start, idle_period_end),
*             adopted policy (policy), parameters for timeout policy (timeout) and
*             history-based policy (hist_alpha, prediction_threshold1, prediction_threshold2)
* Behavior:   if current time is earlier than beginning of idle period, go to ACTIVE state;
*             else, if policy is TIMEOUT go to IDLE after timeout point,
*             else, if policy is HISTORY_BASED apply policy (to be implemened)
* Return:     determined current state
*/
int decide_state(int curr_time, double idle_period_start, double idle_period_end, int policy, int timeout, double *hist_alpha, double *history,  double prediction_threshold1, double prediction_threshold2, double sleep_timeout){
    int psm_state;

    switch (policy){
	// timeout policy
        case TIMEOUT:
        {
            if(curr_time < idle_period_start){
	      // in active period
              psm_state = ACTIVE;
            }
            else if (curr_time - idle_period_start > timeout){
                /*  Lab2 */
                if (curr_time - idle_period_start > sleep_timeout)
                    psm_state = SLEEP;
                else
                    // after timeout point
                    psm_state = IDLE;
            }
            break;
        }

        case HISTORY_BASED:{

	    // history-based predictive policy
            if(curr_time < idle_period_start){
		// in active period
                psm_state = ACTIVE;
            }
            else {
                double value_prediction = 0.0;

                // MV begin
                #ifdef WIN_1
                for(int i = 0; i < POLYN; i++){
                    // in this case window size equal to 1-> only 1 element in the vector
                    value_prediction += hist_alpha[i] * pow(history[0], POLYN - i);
                }
                value_prediction += hist_alpha[POLYN];

                #elif WIN_2
                // MATLAB's polyfitn returns the following data, where X1 is the latest Tidle i.e. Tidle[i-1]:
                // ex: Coefficients: [-0.1203 0.2593 2.4246 -0.1499 -0.8420 4.1695] yield
                //  -0.12031*X1^2 + 0.25933*X1*X2 + 2.4246*X1 - 0.14993*X2^2 - 0.84198*X2 + 4.1695
                // N.B. history[SIZE-1] stores the lastest value (see emulator func). So history[1] is X1 and history[0] is X2
                value_prediction = hist_alpha[0]*pow(history[1], 2) + hist_alpha[1]*history[1]*history[0] +
                hist_alpha[2]*history[1] + hist_alpha[3]*pow(history[0], 2) + hist_alpha[4]*history[0] + hist_alpha[5];

                #elif WIN_3
                // ex: Coefficients: [2.2308 4.8647 5.1769 -2.3807 2.8361 5.1821 -3.0187 2.9135 -3.2373 2.9472] yield
                // 2.2308*X1^2 + 4.8647*X1*X2 + 5.1769*X1*X3 - 2.3807*X1 + 2.8361*X2^2 + 5.1821*X2*X3
                // - 3.0187*X2 + 2.9135*X3^2 - 3.2373*X3 + 2.9472
                // history[2] -> X1, history[1] -> X2, history[0] -> X3
                value_prediction =  hist_alpha[0]*pow(history[2], 2) + hist_alpha[1]*history[2]*history[1] +
                hist_alpha[2]*history[2]*history[0] + hist_alpha[3]*history[2] + hist_alpha[4]*pow(history[1], 2) +
                hist_alpha[5]*history[1]*history[0] + hist_alpha[6]*history[1] + hist_alpha[7]*pow(history[0], 2) +
                hist_alpha[8]*history[0] + hist_alpha[9];

                #endif

                // MV end
                // DEBUGGING
                //printf("\nvalue prediction: %f\n", value_prediction);
                //if(value_prediction > 55.0)
                   //printf("\nvalue prediction: %f\n", value_prediction);
                // END DEBUGGING
                if(value_prediction > prediction_threshold2)
                    psm_state = SLEEP;
                else if(value_prediction > prediction_threshold1)
                    psm_state = IDLE;
                else
                    psm_state = ACTIVE;
            }
            break;
        }
        case ADAPTIVE:
        {
            break;
        }

    }
    return psm_state;
}

/*
* Function:   emulator
* Parameters: pointer to workload file, characteristics of the power state machine (numstate, psm_energy, psm_time, psm_name, ppow),
              characteristics of the policy (policy, timeout for timeout policy, hist_alpha, prediction_threshold1 and prediction_threshold2
              for history based policy)
* Behavior:   for all timing instants from current time to end of idle period, determines next state and energy consumption
*             depending on new state and on whether a transition occurred
* Return:
*/
void emulator(char *fwl, int numstate, double *psm_energy, double *psm_time, char *psm_name, double *ppow, int policy, double timeout, double *hist_alpha, double prediction_threshold1, double prediction_threshold2, double sleep_timeout) // int index
{
	FILE *fp;
	fp = fopen(fwl, "r");
	if(!fp)
	{
		printf("error: can't open file %s!\n", fwl);
		return;
	}

    int idle_period_start, idle_period_end;//, prev_idle_period_end = 0;
    double history[HIST_WIND_SIZE];

    int i;
    for (i=0; i<HIST_WIND_SIZE; i++){
        history[i] = 0;
    }

    int curr_time;
    int prev_psm_state;
    int curr_psm_state;
    int time_active = 0;
    int time_waiting = 0;
    int time_idle = 0;
    int time_sleep = 0;
    int time_idle_org = 0;
    int num_of_transtion = 0;
    double E_transition;
    double E_transition_total = 0;
    double t_transition = 0;
    double E_consumped;

    // initial state
    curr_time = 1;
    E_consumped = 0;
    E_transition = 0;
    prev_psm_state = 0;    curr_psm_state = 0;

    // while there are idle intervals in the workload files
    while(fscanf(fp, "%d%d", &idle_period_start, &idle_period_end) == 2 && idle_period_start < MAX_EMUL_TIME)
	{
	// calculate current idle time
	time_idle_org += idle_period_end - idle_period_start;

        //update history window
        int j;
        for (j=0; j<HIST_WIND_SIZE-1; j++){
            history[j] = history[j+1];
        }
        history[HIST_WIND_SIZE-1] = idle_period_end - idle_period_start;
        // DEBUGGING
        //for (j=0; j<HIST_WIND_SIZE; j++)
            //printf("j=%d HISTORY[j]  == %f\n",j,history[j]);
        // For current (active + idle)period
        for(i = curr_time; i<idle_period_end;i++){
            // obtain current psm state
            curr_psm_state = decide_state(i, idle_period_start, idle_period_end, policy, timeout,
                                          hist_alpha, history, prediction_threshold1, prediction_threshold2, sleep_timeout);

            // calculate energy
            if (prev_psm_state != curr_psm_state){
		// if transition occured
                E_transition = psm_energy[prev_psm_state*numstate+ curr_psm_state];
		//add transition energy

                if(E_transition < 0){
                    printf("error: prohibited transition!\n");
                    break;
                }
                else{
		    // update energy consumption with energy consumption determined by state plus transition
                    E_consumped += ppow[curr_psm_state]*POWER_UNIT*TIME_UNIT + E_transition * ENERGY_UNIT;
		    // update total transition cost
                    E_transition_total += E_transition * ENERGY_UNIT;
		    // update time spent in transitions
                    t_transition += psm_time[prev_psm_state*numstate+ curr_psm_state] *TIME_UNIT;
		    // increment number of transitions
                    num_of_transtion++;
		    // update previous state for next time estimation
                    prev_psm_state = curr_psm_state;
                }
            }
            else{
		// update energy consumption with energy consumption determined by state
                E_consumped += ppow[curr_psm_state] *POWER_UNIT * TIME_UNIT;
            }
            // cumulate psm times
            switch (curr_psm_state){
                case ACTIVE:
                {
                    if (i >= idle_period_start) time_waiting++;
                    else time_active++;
                    break;
                }
                case IDLE: time_idle++;
                    break;
                case SLEEP: time_sleep++;
                    break;
            }

        }
        // update current time to end of idle period
        curr_time = idle_period_end;

    }
    for (i=0; i<curr_time; i++){
        //printf("%d ", psm_state[i]);
    }

    //printf("%d\n", curr_time);

    // print statistics
    printf("Active time in profile = %.3fms\n",(curr_time - time_idle_org)*TIME_UNIT*1000);
    printf("Idle time in profile = %.3fms\n\n",time_idle_org*TIME_UNIT*1000);

    printf("Total time = %.3fms\n",(curr_time*TIME_UNIT+t_transition)*1000);
    printf("Active time = %.3fms\n",time_active*TIME_UNIT*1000);
    printf("Timeout wating time = %.3fms\n", time_waiting*TIME_UNIT*1000);
    printf("Idle time = %.3fms\n",time_idle*TIME_UNIT*1000);
    printf("Sleep time = %.3fms\n",time_sleep*TIME_UNIT*1000);
    printf("Time overhead for transition = %.3fms\n",t_transition*1000);

    printf("# of transitions = %d, total E_transtion = %.3fmJ\n", num_of_transtion, E_transition_total * 1000);
    printf("Energy w/o DPM = %.3fmJ, Energy w DPM = %.3fmJ\n",
           ppow[ACTIVE]*POWER_UNIT*curr_time*TIME_UNIT * 1000, E_consumped * 1000);
    printf("%2.1f percent of energy saved.\n",
           ((ppow[ACTIVE]*POWER_UNIT* curr_time*TIME_UNIT) - E_consumped)/(ppow[ACTIVE]*POWER_UNIT*curr_time*TIME_UNIT)*100);
    if(fp)
		fclose(fp);
}

/*
* Function:   main
*/
int main(int argc, char *argv[])
{
	char fwl[MAXPATH], fpsm[MAXPATH];
	int policy = 0;
	strcpy(fwl, "wl.txt");
	strcpy(fpsm, "psm.txt");
	double timeout = 0;
	double hist_alpha[POLYN + 1];  // MV
	double prediction_threshold1;
	double prediction_threshold2;
    //MV
    double sleep_timeout = 0;
    // end MV

	if(!parse_arg(argc, argv, fwl, fpsm, timeout, hist_alpha, prediction_threshold1, prediction_threshold2, policy, sleep_timeout))
	{
		printf("error arguments!\n");
		return 0;
	}

	double *psm_energy = NULL, *psm_time = NULL;
	char *psm_name = NULL;
	int numstate = 0;
	double *ppow = NULL;

	if(!init(fpsm, psm_energy, psm_time, psm_name, ppow, numstate))
		return 0;

	switch(policy){
	    case TIMEOUT:
	    {
		printf("Policy = timeout, Timeout = %.0fus, Sleep timeout = %.0fus\n", timeout, sleep_timeout);
		break;
	    }
	    case HISTORY_BASED:
	    {
        printf("Policy = history based,coefficients = {");
        for (int i = 0; i < POLYN+1; i++) {
          printf("%.2f ", hist_alpha[i]);
        }
        printf("threshold = {%.2f, %.2f}\n", prediction_threshold1, prediction_threshold2 );

	/*	printf("Policy = history based,coefficients = {%.2f %.2f %.2f %.2f %.2f}, threahold = {%.2f, %.2f}\n",
		      hist_alpha[0], hist_alpha[1], hist_alpha[2], hist_alpha[3], hist_alpha[4],
		      prediction_threshold1, prediction_threshold2);*/
		break;
	    }
	    case ADAPTIVE:
	    {
		printf("Policy = adaptive\n");
		break;
	    }
	}

	emulator(fwl, numstate, psm_energy, psm_time,psm_name, ppow, policy, timeout, hist_alpha, prediction_threshold1, prediction_threshold2, sleep_timeout);
	if(psm_energy)
		free(psm_energy);
	if(psm_time)
		free(psm_time);
	if(psm_name)
		free(psm_name);
	if(ppow)
		free(ppow);
//	getchar();
	return 0;

}
