#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"
//#include "LED color.h"
#include <math.h>

//GPIO_config
void GPIO_config(){
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);				/*+AFIO是STM32F10x微控制器的一個重要模塊，負責管理微控制器引腳的複用和中斷線路。
																																如果需要使用AFIO功能，就需要先開啟相關的時鐘才能使其正常運作。
																																因此，在本程式中需要使用GPIO複用功能所以需要先開啟相關時鐘。*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//開啟GPIOB時鐘
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);	//+把GPIO的腳位複用TIM，PWM訊號輸出。GPIO_PinRemapConfig()函數允許開發人員將某些引腳重新映射（mapping）到不同的功能模塊。
	
	GPIO_InitTypeDef GPIO_InitStruct;	//宣告結構變數
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;	//LED綠色腳位            

  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;  

  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;	/*微控制器上生成 PWM（脈衝寬度調製）信號時，需要配置相應的 GPIO（通用輸入/輸出）引腳。在設定 PWM 時，
																								我們通常會選擇 GPIO_Mode_AF_PP（Alternate Function Push-Pull）模式，原因如下：Alternate Function（AF）：由於 PWM 信號是由微控制器的內部周邊設備（例如定時器）生成的，
																								我們需要將相應的 GPIO 引腳配置為 Alternate Function 模式，以便將內部 PWM 信號輸出到外部。AF 模式允許微控制器的引腳用於特定的功能，而不僅僅是通用的輸入/輸出。
																								Push-Pull（PP）：在 GPIO 的輸出類型中，有兩種模式：Push-Pull 和 Open-Drain。對於 PWM 應用，我們通常選擇 Push-Pull 模式。在 Push-Pull 模式下，當引腳輸出高電平時，
																								它會連接到 Vcc（正電源），而輸出低電平時，它會連接到 GND（地）。這種模式可以提供更穩定的輸出電平，使 PWM 信號的波形更加清晰，同時提供較高的驅動能力。*/
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;  //LED藍色腳位 
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5; 	//LED紅色腳位
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}
//TIM_config
void TIM_config(){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//TIM3定時器時鐘打開
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;		//宣告結構變數
	
	TIM_TimeBaseInitStruct.TIM_Prescaler=15; //除頻(系統頻率/TIM_Prescaler+1)          

  TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上計數模式      

  TIM_TimeBaseInitStruct.TIM_Period=255; //設定ARR(count最高值)          

  //TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);//設定計時器計數模組
	
	//PWM

	TIM_OCInitTypeDef TIM_OCInitStruct;	//宣告結構變數
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;

																										/*請參考PPT筆記說明1
																										TIM_OCMode_Timing:此模式下，定時器的輸出通道只能產生一個定時脈衝信號。
																																				當計數器的值等於比較器的值時，輸出通道會短暫地輸出一個高電平脈衝，然後恢復低電平狀態。該模式通常用於產生單次的定時脈衝信號。
																																				
																										TIM_OCMode_Active :此模式下，當計數器的值小於比較器的值時，輸出通道維持高電平狀態；當計數器的值大於比較器的值時，輸出通道恢復低電平狀態。
																																				該模式通常用於控制電路元件的開關，例如控制電機的啟動和停止。
																																				
																										TIM_OCMode_Inactive:此模式下，當計數器的值小於比較器的值時，輸出通道維持低電平狀態；當計數器的值大於比較器的值時，輸出通道恢復高電平狀態。
																																				該模式通常用於控制電路元件的開關，例如控制 LED 燈的亮度調節。
																																				
																										TIM_OCMode_Toggle:此模式下，當計數器的值等於比較器的值時，輸出通道的電平狀態會切換一次，從而產生一個脈衝信號。
																																			該模式通常用於產生交替的脈衝信號，例如控制繼電器的開關。  
																																			
																										TIM_OCMode_PWM1:此模式下，當計數器的值小於比較器的值時，輸出通道維持高電平狀態；當計數器的值大於比較器的值時，輸出通道恢復低電平狀態。
																																		在一個計數週期內，高電平脈衝的持續時間 (即脈衝寬度) 取決於比較器的值。因此，可以通過改變比較器的值來改變 PWM 信號的脈寬，
																																		從而控制輸出通道的平均電壓和電流。此模式通常用於控制直流馬達的轉速和 LED 燈的亮度調節等應用。   
																																		
                                                    TIM_OCMode_PWM2:此模式下，當計數器的值小於比較器的值時，輸出通道維持低電平狀態；當計數器的值大於比較器的值時，輸出通道恢復高電平狀態。
																																		在一個計數週期內，高電平脈衝的持續時間 (即脈衝寬度) 也取決於比較器的值。
																																		和 PWM1 模式不同的是，此模式下高電平脈衝的起始時間是固定的，且比較器的值必須小於計數器的最大值，否則輸出通道永遠處於高電平狀態。
																																		此模式通常用於產生互補式 PWM 信號，例如用於直流馬達的前進和後退控制。*/    


	

  TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   //始能輸出


  TIM_OCInitStruct.TIM_Pulse=0; /*請參閱PPT筆記。設置初始CCR=0，後面CCR依設定而改，即一開始count數>比較器數值。*/        

 TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;    /*TIM_OCPolarity 是一個枚舉型別，表示定時器 (Timer) 模組的輸出通道的極性，即脈衝信號的電平。
																													TIM_OCPolarity_Low 是其中的一個常數，表示輸出通道的脈衝信號電平為低電平。
																													因此，當設置 TIM_OCPolarity 為 TIM_OCPolarity_Low 時，該輸出通道的脈衝信號會在低電平時有效。
																													例如，若將 TIM3 的通道 1 設定為 PWM1 模式，並設置 TIM_OCPolarity 為 TIM_OCPolarity_Low，
																					                則該通道輸出的 PWM 脈衝信號會在計數器值小於比較器值時維持低電平，並在計數器值大於比較器值時恢復為高電平。*/
 	
	
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);	    /*初始化定時器(Timer)模組的第二個輸出通道 (Output Compare 2, 簡寫為 OC2) 的參數設定。
																						不同GPIO輸出腳位會有規定的配置通道，在此TIM3模組配合GPIO B5 (紅光)需使用OC2。*/
																						
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);/*?????????????? TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable) 這行程式碼的意思是將 TIM3 計時器的通道 2 的寄存器 (CCR2) 值的預裝載 (preload) 功能啟用。
																											啟用預裝載功能會在新的比較值 (compare value) 寫入 CCR2 寄存器之前，先將新的比較值寫入 TIM3 的影子寄存器 (shadow register)，等到計時器更新時再將影子寄存器中的比較值載入 CCR2 寄存器。
																											這個功能可以避免因為比較值的改變而導致的計時器工作不穩定。*/
	
	TIM_OC3Init(TIM3,&TIM_OCInitStruct); 
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM3,&TIM_OCInitStruct);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);/*啟用ARR*/

//使能定时器
  TIM_Cmd(TIM3, ENABLE);/*啟用TIM定時器*/
 	
}

//設定個顏色亮度

void setcolorvalue(uint8_t r,uint8_t g,uint8_t b){	/*請參考PPT筆記1和4   在上面 TIM_TimeBaseInitStruct.TIM_Period=255; 就是count最高可達=255(ARR)
																											這邊設定的r,g,b，分別控制三種顏色LED的PWM寬度。*/
	TIM3->CCR2=r; //+
	
	TIM3->CCR3=g; //+
	
	TIM3->CCR4=b; //+

}

void Delay(int us){ //delay function
int count=0;
	while(us--){
		count=9;
		while(count--);
	}

}

int iii=0;
int main(void)
{
	float asa=0;
	float ddd=0;
	
	while(iii==1){
	}
	
	GPIO_config();
	
	TIM_config();
	
	while(1){
		
		asa=sin(ddd)*128+128; /*請參考PPT筆記9。
														sin是正旋波所以sin(ddd)的值會在-1~1之間變化
														，因此sin(ddd)*128+128就可以在0~256之間變化。*/
		ddd+=0.01;
		//iii=asa;
		setcolorvalue(asa,0,asa);
		//iii+=20;
		Delay(0xFF);
		
		//if(iii==255){
			//iii=0;
		}
		
	
	return 0;
	}
	
	
	


  

void SystemInit(void)
{	
}




/*********************************************END OF FILE**********************/
