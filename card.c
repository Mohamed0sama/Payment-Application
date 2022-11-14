/*
 * card.c
 *
 *  Created on: Oct 12, 2022
 *      Author: maham
 */
#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "std_types.h"
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	enum EN_cardError_t EN_CardHolderName_State = WRONG_NAME;
	uint8_t i = 0;
	uint8_t count = 0;
	while (EN_CardHolderName_State == WRONG_NAME) {
		printf("Please enter Card Holder Name \n");
		gets(cardData->cardHolderName);
		while (cardData->cardHolderName[i] != '\0') {
			if (cardData->cardHolderName[i] != ' ') {
				count++;
			}
			i++;
		}
		if (count < 20 || count > 24) {
			EN_CardHolderName_State = WRONG_NAME;
			printf("WRONG NAME | Name must be between 20 and 24 characters\n");
		}
		else {
			EN_CardHolderName_State = OK;
			printf("CORRECT NAME\n");
		}
		i = 0;
		count = 0;
	}
	return EN_CardHolderName_State;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	enum EN_cardError_t EN_CardExpiryDate_State = WRONG_EXP_DATE;
	uint8_t i = 0;
	uint8_t M[2] = { 0 };
	uint8_t Y[2] = { 0 };
	uint8_t Month_Terminal, Year_Terminal;
	while (EN_CardExpiryDate_State == WRONG_EXP_DATE) {
		printf("Please enter Expiration Date in the Following Format MM/YY\n");
		gets(cardData->cardExpirationDate);
		M[0] = cardData->cardExpirationDate[0];
		M[1] = cardData->cardExpirationDate[1];
		Y[0] = cardData->cardExpirationDate[3];
		Y[1] = cardData->cardExpirationDate[4];
		Month_Terminal = atoi(M);
		Year_Terminal = atoi(Y);
		if (cardData->cardExpirationDate[2] == '/') {
			if (Month_Terminal < 13 && Year_Terminal >= 22) {
				EN_CardExpiryDate_State = OK;
				printf("CORRECT FORMAT\n");
			}
			else {
				EN_CardExpiryDate_State = WRONG_EXP_DATE;
				printf("Please enter date in the correct format\n");
			}
		}
		else EN_CardExpiryDate_State = WRONG_EXP_DATE;
	}
	return EN_CardExpiryDate_State;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	enum EN_cardError_t EN_CardPAN_State = WRONG_PAN;
	uint8_t i = 0;
	while (EN_CardPAN_State == WRONG_PAN) {
		printf("Please enter PAN Number | NB: PAN Number must be between 16 and 19 digits\n");
		gets(cardData->primaryAccountNumber);
		while (cardData->primaryAccountNumber[i] != '\0') {
			i++;
		}
		if (i >= 16 && i <= 19) EN_CardPAN_State = OK;
		else EN_CardPAN_State = WRONG_PAN;
		i = 0;
	}
	return EN_CardPAN_State;
}
