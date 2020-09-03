/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    lowpower.c
 *----------------------------------------------------------------------------

#include "MKL46Z4.h"                    // Device header
#include "lowpower.h"

enum run_mode
{
	RUN_MODE_NORMAL = 0ul,
	RUN_MODE_VLP = 2ul,
};

enum stop_mode
{
	STOP_MODE_NORMAL = 0ul,
	STOP_MODE_VLP = 2ul,
	STOP_MODE_LL = 3ul,
	STOP_MODE_VLL = 4ul,
};

void lowPowerInitialize(void)
{
  /* Write to PMPROT in System mode controller to allow LLS power modes */
	SMC->PMPROT = SMC_PMPROT_AVLP_MASK; /*Allow Very-low-power mode*/
	SMC->PMCTRL = SMC_PMCTRL_RUNM(RUN_MODE_VLP);
	
	return;
}

void enterVLPMode(void)
{
int volatile dummyread;

	/*Set the STOPM  bits to select the LLS low-power mode*/
	SMC->PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
	SMC->PMCTRL |= SMC_PMCTRL_STOPM(STOP_MODE_VLP); 
	
	/* Wait for write to complete to SMC before stopping core */
	dummyread = SMC->PMCTRL; /*wait for write to complete to SMC before stopping core */
	dummyread++;
	
	SCB->SCR &= (~SCB_SCR_SLEEPDEEP_Msk); /* Set the SLEEPDEEP bit to enable deep sleep mode */
	SCB->SCR |= SCB_S
	__wfi();
	return;
}
