/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v30x_it.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2022/01/18
* Description        : Main Interrupt Service Routines.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include <WCHNET.h>
#include "eth_driver.h"
#include "ch32v30x_it.h"

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void ETH_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI9_5_IRQHandler(void) __attribute__((interrupt()));

u8 publishValid = 0;
u16 timeCnt = 0;
/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
    printf("HardFault_Handler\r\n");

    printf("mepc  :%08x\r\n", __get_MEPC());
    printf("mcause:%08x\r\n", __get_MCAUSE());
    printf("mtval :%08x\r\n", __get_MTVAL());
    while(1);
}

/*********************************************************************
 * @fn      EXTI9_5_IRQHandler
 *
 * @brief   This function handles GPIO exception.
 *
 * @return  none
 */
void EXTI9_5_IRQHandler(void)
{
    ETH_PHYLink( );
    EXTI_ClearITPendingBit(EXTI_Line7);     /* Clear Flag */
}

/*********************************************************************
 * @fn      ETH_IRQHandler
 *
 * @brief   This function handles ETH exception.
 *
 * @return  none
 */
void ETH_IRQHandler(void)
{
    WCHNET_ETHIsr();
}

/*********************************************************************
 * @fn      TIM2_IRQHandler
 *
 * @brief   This function handles TIM2 exception.
 *
 * @return  none
 */
void TIM2_IRQHandler(void)
{
    if(!publishValid){            //每隔20s置一下publishValid标志
        timeCnt += 10;
        if(timeCnt > 20000){
            publishValid = 1;
            timeCnt = 0;
        }
    }
    WCHNET_TimeIsr(WCHNETTIMERPERIOD);
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
