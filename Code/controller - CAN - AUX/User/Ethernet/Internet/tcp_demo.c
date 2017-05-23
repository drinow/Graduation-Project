/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP ��ʾ����
* @attention  
************************************************************************************************
**/

#include <stdio.h>
#include <string.h>
#include "tcp_demo.h"
#include "W5500_conf.h"
#include "w5500.h"
#include "socket.h"

#include "including.h"

uint8 buff[2048];				                              	         /*����һ��2KB�Ļ���*/

/**
*@brief		TCP Server�ػ���ʾ������
*@param		��
*@return	��
*/
void do_tcp_server(void)
{	
	uint16 len=0;   
  static u8 i=0;
	switch(getSn_SR(SOCK_TCPS))											            	/*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:													                  /*socket���ڹر�״̬*/
			socket(SOCK_TCPS ,Sn_MR_TCP,local_port,Sn_MR_ND);	        /*��socket*/
      printf("SOCK_CLOSED\r\n");
		  break;     
    
		case SOCK_INIT:														                  /*socket�ѳ�ʼ��״̬*/
			listen(SOCK_TCPS);												                /*socket��������*/
      printf("SOCK_INIT\r\n");
		  break;
		
		case SOCK_ESTABLISHED:												              /*socket�������ӽ���״̬*/
			if(getSn_IR(SOCK_TCPS) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPS, Sn_IR_CON);								          /*��������жϱ�־λ*/
			}
			len=getSn_RX_RSR(SOCK_TCPS);									            /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0)
			{
				recv(SOCK_TCPS,buff,len);								              	/*��������Client������*/
				buff[len]=0x00; 											                  /*����ַ���������*/
				printf("%s\r\n",buff);
        TCP_DealData(buff,len);
//				send(SOCK_TCPS,buff,len);									              /*��Client��������*/
		  }
      if(tempflag)
      {
        tempflag=0;
        switch(i)
        {
        case 1:TCP_SendDetector1F();break;
        case 2:TCP_SendDetector2F();break;
        case 3:TCP_SendDetector3F();break;
        case 4:
//          if(RestFire||LocalFire)
//          {
//            if(FiredSended==0)
//            {
//              if(FiredNum!=0)
//              {
//                TCP_SendFire(FiredNum&0xFF,(FiredNum&0xFF00)>>8);
//                FiredSended=1;
//              }
//            }
//          }
          break;
        default:break;
        }
        i++;
        if(i>3)i=0;
      }
		  break;
		
		case SOCK_CLOSE_WAIT:												                /*socket���ڵȴ��ر�״̬*/
			close(SOCK_TCPS);
      printf("SOCK_CLOSE_WAIT\r\n");
		  break;
	}
}

/**
*@brief		TCP Client�ػ���ʾ������
*@param		��
*@return	��
*/
void do_tcp_client(void)
{	
   uint16 len=0;	

	switch(getSn_SR(SOCK_TCPC))								  				         /*��ȡsocket��״̬*/
	{
		case SOCK_CLOSED:											        		         /*socket���ڹر�״̬*/
			socket(SOCK_TCPC,Sn_MR_TCP,local_port++,Sn_MR_ND);
			printf("SOCK_CLOSED...\r\n");
		  break;
		
		case SOCK_INIT:													        	         /*socket���ڳ�ʼ��״̬*/
			connect(SOCK_TCPC,remote_ip,remote_port);                /*socket���ӷ�����*/ 
			printf("connecting the server...\r\n");
		  break;
		
		case SOCK_ESTABLISHED: 												             /*socket�������ӽ���״̬*/
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON); 							         /*��������жϱ�־λ*/
			}
		
			len=getSn_RX_RSR(SOCK_TCPC); 								  	         /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0)
			{
				recv(SOCK_TCPC,buff,len); 							   		         /*��������Server������*/
				buff[len]=0x00;  											                 /*����ַ���������*/
//				printf("%s\r\n",buff);
				send(SOCK_TCPC,buff,len);								     	         /*��Server��������*/
			}  
		  break;
			
		case SOCK_CLOSE_WAIT: 											    	         /*socket���ڵȴ��ر�״̬*/
      printf("SOCK_CLOSE_WAIT...\r\n");
			close(SOCK_TCPC);
		  break;

	}
}

