#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include "stdio.h"
#include "stdlib.h"
#include "std_types.h"
#include "terminal.h"
#include "string.h"
#include "card.h"



EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	enum EN_terminalError_t TransactionDate_State = WRONG_DATE;
	uint8_t TransactionDate_Length;
	uint8_t M[2]={0}; 
	uint8_t Y[4] = { 0 };
	uint8_t D[2]={0};
	uint8_t Month_Terminal;
	uint8_t Day_Terminal;
	uint16_t Year_Terminal;
	while (TransactionDate_State == WRONG_DATE) {
		printf("Please enter Transaction Date in the following Format DD/MM/YYYY\n");
		scanf("%s", &termData->transactionDate);
		M[0] = termData->transactionDate[3]; 
		M[1] = termData->transactionDate[4]; //save MM values in Array Month_Terminal
		Y[0] = termData->transactionDate[6]; 
		Y[1] = termData->transactionDate[7]; //save YY Values in Array Year_Terminal
		Y[2] = termData->transactionDate[8]; 
		Y[3] = termData->transactionDate[9];
		D[0] = termData->transactionDate[0]; 
		D[1] = termData->transactionDate[1]; //save DD Values in Array Day_Terminal
		Day_Terminal = atoi(D); 
		Month_Terminal = atoi(M); 
		Year_Terminal = atoi(Y);//Convert each Array(D/M/Y) into fixed numbers (Day_Terminal/Month_Terminal_Terminal/Year_Terminal)
		TransactionDate_Length = strlen(termData->transactionDate);	//get Transaction Length
		if (TransactionDate_Length == 10) {
			if (termData->transactionDate[2] == '/' && termData->transactionDate[5] == '/') {
				if (Year_Terminal >= 2022) {
					if (Month_Terminal <= 12 && Month_Terminal > 0) {
						if (Day_Terminal > 0 && Day_Terminal <= 31) {
							return T_OK;
							printf("OK\n");
						}
						else TransactionDate_State = WRONG_DATE;
					}
					else TransactionDate_State = WRONG_DATE;
				}
				else TransactionDate_State = WRONG_DATE;
			}
			else TransactionDate_State = WRONG_DATE;
		}
		else TransactionDate_State = WRONG_DATE;
	}
	return TransactionDate_State;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
	uint8_t cardMonthArr[2],cardYearArr[2];
	uint8_t terminalMonthArr[2],terminalYearArr[2];
	uint16_t card_year, terminal_year, card_month, terminal_month;

	cardMonthArr[0] = cardData.cardExpirationDate[0];
	cardMonthArr[1] = cardData.cardExpirationDate[1];
	cardYearArr[0] = cardData.cardExpirationDate[3];
	cardYearArr[1] = cardData.cardExpirationDate[4];


	terminalMonthArr[0] = termData.transactionDate[3];
	terminalMonthArr[1] = termData.transactionDate[4];
	terminalYearArr[0] = termData.transactionDate[8];
	terminalYearArr[1] = termData.transactionDate[9];

	card_year = atoi(cardYearArr);
	terminal_year = atoi(terminalYearArr);
	card_month = atoi(cardMonthArr);
	terminal_month = atoi(terminalMonthArr);

	if (termData.transactionDate[6] == '2' && termData.transactionDate[7] == '0') {
		if (terminal_year < card_year) {
			return OK;
		}
		else if (terminal_year == card_year) {
			if (card_month > terminal_month) {
				return OK;
			}
			else return EXPIRED_CARD;
		}
		else return EXPIRED_CARD;
	}
	else return EXPIRED_CARD;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	EN_terminalError_t TransactionState = INVALID_AMOUNT;
	while (TransactionState == INVALID_AMOUNT) {
		printf("Please enter Transaction Amount\n");
		scanf("%f", &(termData->transAmount));
		if (termData->transAmount > 0) {
			TransactionState = T_OK;
		}
		else TransactionState = INVALID_AMOUNT;
	}
	return TransactionState;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->transAmount > termData->maxTransAmount || termData->transAmount == 0) {
		return INVALID_MAX_AMOUNT;
	}
	else return T_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	EN_terminalError_t MaxAmount_State = INVALID_MAX_AMOUNT;
	while (MaxAmount_State == INVALID_MAX_AMOUNT) {
		printf("Please enter Max Amount\n");
		scanf("%f", &termData->maxTransAmount);
		if (termData->maxTransAmount <= 0) {
			MaxAmount_State = INVALID_MAX_AMOUNT;
		}
		else {
			MaxAmount_State = T_OK;
			printf("Max amount Set = %f\n", termData->maxTransAmount);
		}
	}
	return MaxAmount_State;
}
