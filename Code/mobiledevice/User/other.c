#include "other.h"

void SPG0(void)
{
  delay_ms(1000);
  printf("DR3\r\n");delay_ms(25);
  printf("BPIC(0,0,0,6)\r\n");delay_ms(500);
  printf("DS48(95,10,'������У׼...',1,0)\r\n");delay_ms(25);
  printf("DS48(95,330,'����ס�豸',1,0)\r\n");delay_ms(25);
  printf("DS48(95,400,'��8�ֻ��ƶ���',1,0)\r\n");delay_ms(25);
  printf("DS48(95,470,'��C1���˳�',1,0)\r\n");delay_ms(25);
}
void SPG1(void)
{
  printf("BPIC(0,0,0,6)\r\n");delay_ms(500);
  printf("DS48(95,10,'�ൺ����ѧ',1,0)\r\n");delay_ms(25);
  printf("PIC(0,70,5)\r\n");delay_ms(100);
  printf("PIC(130,400,3)\r\n");delay_ms(100);
  printf("METE(0,130,400,70,3,3,6,0)\r\n");delay_ms(100);
  printf("DS48(20,650,'����:',1,0)\r\n");delay_ms(100);
  printf("DS48(20,750,'����:',1,0)\r\n");delay_ms(100);
//  METS(0,0)
}

unsigned char g_ucTempbuf[20];  
u8 CardID=0;
void RD_Card(void)
{
   unsigned char status,i;
   status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
   if (status != MI_OK)
   {    return;    }
     
   status = PcdAnticoll(g_ucTempbuf);//����ײ
   if (status != MI_OK)
   {    return;    }
    
  CardID=g_ucTempbuf[0];
    printf("�����кţ�%x",CardID);	//�����ն���ʾ,
////    for(i=0;i<4;i++)
////    {
////      printf("%X ",g_ucTempbuf[i]);
////    }
//    printf("\r\n");
}

