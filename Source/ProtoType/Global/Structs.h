#pragma once
#include <iostream>
#include <string.h>
#include <vector>

struct SSelectorType
{
    int Type = 0;
    int MaxElIndex = 0;
    float Elemental[8];
    int error = 0;
};

struct APData
{
    //std::vector<int> buildingName;
    float latitude;
    float longitude;
    int floorInfo;
    int ApartIndex;
};

struct SaleData
{
    int articleNo;
    int dealOrWarrantPrc;
    //std::vector<int> tradeTypeName;
    //std::vector<int> realtorName;
    int floorInfo;
    int ApartIndex;
};