#pragma once
#include <iostream>
#include <string.h>
#include <vector>

struct SSelectorType
{
    int Type = 0;
    int MaxElIndex = 0;
    float Elemental[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int error = 0;
};

struct APData
{
    char buildingName[200];
    float latitude;
    float longitude;
    int floorInfo;
    int ApartIndex;
};

struct SaleData
{
    UINT articleNo;
    int dealOrWarrantPrc;
    char tradeTypeName[200];
    char realtorName[200];
    int floorInfo;
    int ApartIndex;
};