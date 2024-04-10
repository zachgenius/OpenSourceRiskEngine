/*
 Copyright (C) 2016 Quaternion Risk Management Ltd.
 All rights reserved.

 This file is part of ORE, a free-software/open-source library
 for transparent pricing and risk analysis - http://opensourcerisk.org

 ORE is free software: you can redistribute it and/or modify it
 under the terms of the Modified BSD License.  You should have received a
 copy of the license along with this program.
 The license is also available online at <http://opensourcerisk.org>

 This program is distributed on the basis that it will form a useful
 contribution to risk analytics and model standardisation, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
*/

#include <orea/simm/simmconcentrationisdav2_0.hpp>
#include <orea/simm/simmconfigurationisdav2_0.hpp>

#include <ql/math/matrix.hpp>

#include <boost/make_shared.hpp>
#include <boost/algorithm/string/predicate.hpp>

using QuantLib::InterestRateIndex;
using QuantLib::Matrix;
using QuantLib::Real;
using std::string;
using std::vector;

namespace ore {
namespace analytics {

using RiskType = CrifRecord::RiskType;

SimmConfiguration_ISDA_V2_0::SimmConfiguration_ISDA_V2_0(const QuantLib::ext::shared_ptr<SimmBucketMapper>& simmBucketMapper,
                                                         const std::string& name, const std::string version)
    : SimmConfigurationBase(simmBucketMapper, name, version) {

    // Set up the correct concentration threshold getter
    simmConcentration_ = QuantLib::ext::make_shared<SimmConcentration_ISDA_V2_0>(simmBucketMapper_);

    // clang-format off

    // Set up the members for this configuration
    // Explanations of all these members are given in the hpp file
    
    mapBuckets_ = { 
        { RiskType::IRCurve, { "1", "2", "3" } },
        { RiskType::CreditQ, { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "Residual" } },
        { RiskType::CreditVol, { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "Residual" } },
        { RiskType::CreditNonQ, { "1", "2", "Residual" } },
        { RiskType::CreditVolNonQ, { "1", "2", "Residual" } },
        { RiskType::Equity, { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "Residual" } },
        { RiskType::EquityVol, { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "Residual" } },
        { RiskType::Commodity, { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } },
        { RiskType::CommodityVol, { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17" } }
    };

    mapLabels_1_ = {
        { RiskType::IRCurve, { "2w", "1m", "3m", "6m", "1y", "2y", "3y", "5y", "10y", "15y", "20y", "30y" } },
        { RiskType::CreditQ, { "1y", "2y", "3y", "5y", "10y" } },
        { RiskType::CreditNonQ, { "1y", "2y", "3y", "5y", "10y" } },
        { RiskType::IRVol, { "2w", "1m", "3m", "6m", "1y", "2y", "3y", "5y", "10y", "15y", "20y", "30y" } },
        { RiskType::InflationVol, { "2w", "1m", "3m", "6m", "1y", "2y", "3y", "5y", "10y", "15y", "20y", "30y" } },
        { RiskType::CreditVol, { "1y", "2y", "3y", "5y", "10y" } },
        { RiskType::CreditVolNonQ, { "1y", "2y", "3y", "5y", "10y" } },
        { RiskType::EquityVol, { "2w", "1m", "3m", "6m", "1y", "2y", "3y", "5y", "10y", "15y", "20y", "30y" } },
        { RiskType::CommodityVol, { "2w", "1m", "3m", "6m", "1y", "2y", "3y", "5y", "10y", "15y", "20y", "30y" } },
        { RiskType::FXVol, { "2w", "1m", "3m", "6m", "1y", "2y", "3y", "5y", "10y", "15y", "20y", "30y" } }
    };

    mapLabels_2_ = {
        { RiskType::IRCurve, { "OIS", "Libor1m", "Libor3m", "Libor6m", "Libor12m", "Prime", "Municipal" } },
        { RiskType::CreditQ, { "", "Sec" } }
    };

    // Risk weights
    rwRiskType_ = {
        { RiskType::Inflation, 46 },
        { RiskType::XCcyBasis, 20 },
        { RiskType::IRVol, 0.21 },
        { RiskType::InflationVol, 0.21 },
        { RiskType::CreditVol, 0.27 },
        { RiskType::CreditVolNonQ, 0.27 },
        { RiskType::CommodityVol, 0.38 },
        { RiskType::FX, 8.2 },
        { RiskType::FXVol, 0.33 },
        { RiskType::BaseCorr, 20.0 }
    };

    rwBucket_ = {
        {RiskType::CreditQ,
         {{{"1", "", ""}, 85.0},
          {{"2", "", ""}, 85.0},
          {{"3", "", ""}, 73.0},
          {{"4", "", ""}, 49.0},
          {{"5", "", ""}, 48.0},
          {{"6", "", ""}, 43.0},
          {{"7", "", ""}, 161.0},
          {{"8", "", ""}, 238.0},
          {{"9", "", ""}, 151.0},
          {{"10", "", ""}, 210.0},
          {{"11", "", ""}, 141.0},
          {{"12", "", ""}, 102.0},
          {{"Residual", "", ""}, 238.0}}},
        {RiskType::CreditNonQ,
         {{{"1", "", ""}, 140.0},
          {{"2", "", ""}, 2000.0},
          {{"Residual", "", ""}, 2000.0}}},
        {RiskType::Equity,
         {{{"1", "", ""}, 25.0},
          {{"2", "", ""}, 32.0},
          {{"3", "", ""}, 29.0},
          {{"4", "", ""}, 27.0},
          {{"5", "", ""}, 18.0},
          {{"6", "", ""}, 21.0},
          {{"7", "", ""}, 25.0},
          {{"8", "", ""}, 22.0},
          {{"9", "", ""}, 27.0},
          {{"10", "", ""}, 29.0},
          {{"11", "", ""}, 16.0},
          {{"12", "", ""}, 16.0},
          {{"Residual", "", ""}, 32.0}}},
        {RiskType::Commodity,
         {{{"1", "", ""}, 19.0},
          {{"2", "", ""}, 20.0},
          {{"3", "", ""}, 17.0},
          {{"4", "", ""}, 18.0},
          {{"5", "", ""}, 24.0},
          {{"6", "", ""}, 20.0},
          {{"7", "", ""}, 24.0},
          {{"8", "", ""}, 41.0},
          {{"9", "", ""}, 25.0},
          {{"10", "", ""}, 91.0},
          {{"11", "", ""}, 20.0},
          {{"12", "", ""}, 19.0},
          {{"13", "", ""}, 16.0},
          {{"14", "", ""}, 15.0},
          {{"15", "", ""}, 10.0},
          {{"16", "", ""}, 91.0},
          {{"17", "", ""}, 17.0}}},
        {RiskType::EquityVol,
         {{{"1", "", ""}, 0.28},
          {{"2", "", ""}, 0.28},
          {{"3", "", ""}, 0.28},
          {{"4", "", ""}, 0.28},
          {{"5", "", ""}, 0.28},
          {{"6", "", ""}, 0.28},
          {{"7", "", ""}, 0.28},
          {{"8", "", ""}, 0.28},
          {{"9", "", ""}, 0.28},
          {{"10", "", ""}, 0.28},
          {{"11", "", ""}, 0.28},
          {{"12", "", ""}, 0.64},
          {{"Residual", "", ""}, 0.28}}},
    };

    rwLabel_1_ = {
        {RiskType::IRCurve,
            {{{"1", "2w", ""}, 113.0},
             {{"1", "1m", ""}, 113.0},
             {{"1", "3m", ""}, 98.0},
             {{"1", "6m", ""}, 69.0},
             {{"1", "1y", ""}, 56.0},
             {{"1", "2y", ""}, 52.0},
             {{"1", "3y", ""}, 51.0},
             {{"1", "5y", ""}, 51.0},
             {{"1", "10y", ""}, 51.0},
             {{"1", "15y", ""}, 53.0},
             {{"1", "20y", ""}, 56.0},
             {{"1", "30y", ""}, 64.0},
             {{"2", "2w", ""}, 21.0},
             {{"2", "1m", ""}, 21.0},
             {{"2", "3m", ""}, 10.0},
             {{"2", "6m", ""}, 11.0},
             {{"2", "1y", ""}, 15.0},
             {{"2", "2y", ""}, 20.0},
             {{"2", "3y", ""}, 22.0},
             {{"2", "5y", ""}, 21.0},
             {{"2", "10y", ""}, 19.0},
             {{"2", "15y", ""}, 20.0},
             {{"2", "20y", ""}, 23.0},
             {{"2", "30y", ""}, 27.0},
             {{"3", "2w", ""}, 93.0},
             {{"3", "1m", ""}, 93.0},
             {{"3", "3m", ""}, 90.0},
             {{"3", "6m", ""}, 94.0},
             {{"3", "1y", ""}, 97.0},
             {{"3", "2y", ""}, 103.0},
             {{"3", "3y", ""}, 101.0},
             {{"3", "5y", ""}, 103.0},
             {{"3", "10y", ""}, 102.0},
             {{"3", "15y", ""}, 101.0},
             {{"3", "20y", ""}, 102.0},
             {{"3", "30y", ""}, 101.0}}
        }
    };

    // Curvature weights
    curvatureWeights_ = {
        { RiskType::IRVol, { 0.5, 
                             0.5 * 14.0 / (365.0 / 12.0), 
                             0.5 * 14.0 / (3.0 * 365.0 / 12.0), 
                             0.5 * 14.0 / (6.0 * 365.0 / 12.0), 
                             0.5 * 14.0 / 365.0, 
                             0.5 * 14.0 / (2.0 * 365.0), 
                             0.5 * 14.0 / (3.0 * 365.0), 
                             0.5 * 14.0 / (5.0 * 365.0), 
                             0.5 * 14.0 / (10.0 * 365.0), 
                             0.5 * 14.0 / (15.0 * 365.0), 
                             0.5 * 14.0 / (20.0 * 365.0), 
                             0.5 * 14.0 / (30.0 * 365.0) } 
        },
        { RiskType::CreditVol, { 0.5 * 14.0 / 365.0, 
                                 0.5 * 14.0 / (2.0 * 365.0), 
                                 0.5 * 14.0 / (3.0 * 365.0), 
                                 0.5 * 14.0 / (5.0 * 365.0), 
                                 0.5 * 14.0 / (10.0 * 365.0) } 
        }
    };
    curvatureWeights_[RiskType::InflationVol] = curvatureWeights_[RiskType::IRVol];
    curvatureWeights_[RiskType::EquityVol] = curvatureWeights_[RiskType::IRVol];
    curvatureWeights_[RiskType::CommodityVol] = curvatureWeights_[RiskType::IRVol];
    curvatureWeights_[RiskType::FXVol] = curvatureWeights_[RiskType::IRVol];
    curvatureWeights_[RiskType::CreditVolNonQ] = curvatureWeights_[RiskType::CreditVol];

    // Historical volatility ratios
    historicalVolatilityRatios_[RiskType::EquityVol] = 0.65;
    historicalVolatilityRatios_[RiskType::CommodityVol] = 0.80;
    historicalVolatilityRatios_[RiskType::FXVol] = 0.60;

    // Valid risk types
    validRiskTypes_ = {
        RiskType::Commodity,
        RiskType::CommodityVol,
        RiskType::CreditNonQ,
        RiskType::CreditQ,
        RiskType::CreditVol,
        RiskType::CreditVolNonQ,
        RiskType::Equity,
        RiskType::EquityVol,
        RiskType::FX,
        RiskType::FXVol,
        RiskType::Inflation,
        RiskType::IRCurve,
        RiskType::IRVol,
        RiskType::InflationVol,
        RiskType::BaseCorr,
        RiskType::XCcyBasis,
        RiskType::ProductClassMultiplier,
        RiskType::AddOnNotionalFactor,
        RiskType::PV,
        RiskType::Notional,
        RiskType::AddOnFixedAmount
    };

    // Risk class correlation matrix
    riskClassCorrelation_ = {
        {{"", "InterestRate", "CreditQualifying"}, 0.28},
        {{"", "InterestRate", "CreditNonQualifying"}, 0.18},
        {{"", "InterestRate", "Equity"}, 0.18},
        {{"", "InterestRate", "Commodity"}, 0.3},
        {{"", "InterestRate", "FX"}, 0.22},
        {{"", "CreditQualifying", "InterestRate"}, 0.28},
        {{"", "CreditQualifying", "CreditNonQualifying"}, 0.3},
        {{"", "CreditQualifying", "Equity"}, 0.66},
        {{"", "CreditQualifying", "Commodity"}, 0.46},
        {{"", "CreditQualifying", "FX"}, 0.27},
        {{"", "CreditNonQualifying", "InterestRate"}, 0.18},
        {{"", "CreditNonQualifying", "CreditQualifying"}, 0.3},
        {{"", "CreditNonQualifying", "Equity"}, 0.23},
        {{"", "CreditNonQualifying", "Commodity"}, 0.25},
        {{"", "CreditNonQualifying", "FX"}, 0.18},
        {{"", "Equity", "InterestRate"}, 0.18},
        {{"", "Equity", "CreditQualifying"}, 0.66},
        {{"", "Equity", "CreditNonQualifying"}, 0.23},
        {{"", "Equity", "Commodity"}, 0.39},
        {{"", "Equity", "FX"}, 0.24},
        {{"", "Commodity", "InterestRate"}, 0.3},
        {{"", "Commodity", "CreditQualifying"}, 0.46},
        {{"", "Commodity", "CreditNonQualifying"}, 0.25},
        {{"", "Commodity", "Equity"}, 0.39},
        {{"", "Commodity", "FX"}, 0.32},
        {{"", "FX", "InterestRate"}, 0.22},
        {{"", "FX", "CreditQualifying"}, 0.27},
        {{"", "FX", "CreditNonQualifying"}, 0.18},
        {{"", "FX", "Equity"}, 0.24},
        {{"", "FX", "Commodity"}, 0.32}
    };

    // Interest rate tenor correlations (i.e. Label1 level correlations)
    intraBucketCorrelation_[RiskType::IRCurve] = {
        {{"", "2w", "1m"}, 1.0},
        {{"", "2w", "3m"}, 0.79},
        {{"", "2w", "6m"}, 0.67},
        {{"", "2w", "1y"}, 0.53},
        {{"", "2w", "2y"}, 0.42},
        {{"", "2w", "3y"}, 0.37},
        {{"", "2w", "5y"}, 0.3},
        {{"", "2w", "10y"}, 0.22},
        {{"", "2w", "15y"}, 0.18},
        {{"", "2w", "20y"}, 0.16},
        {{"", "2w", "30y"}, 0.12},
        {{"", "1m", "2w"}, 1.0},
        {{"", "1m", "3m"}, 0.79},
        {{"", "1m", "6m"}, 0.67},
        {{"", "1m", "1y"}, 0.53},
        {{"", "1m", "2y"}, 0.42},
        {{"", "1m", "3y"}, 0.37},
        {{"", "1m", "5y"}, 0.3},
        {{"", "1m", "10y"}, 0.22},
        {{"", "1m", "15y"}, 0.18},
        {{"", "1m", "20y"}, 0.16},
        {{"", "1m", "30y"}, 0.12},
        {{"", "3m", "2w"}, 0.79},
        {{"", "3m", "1m"}, 0.79},
        {{"", "3m", "6m"}, 0.85},
        {{"", "3m", "1y"}, 0.69},
        {{"", "3m", "2y"}, 0.57},
        {{"", "3m", "3y"}, 0.5},
        {{"", "3m", "5y"}, 0.42},
        {{"", "3m", "10y"}, 0.32},
        {{"", "3m", "15y"}, 0.25},
        {{"", "3m", "20y"}, 0.23},
        {{"", "3m", "30y"}, 0.2},
        {{"", "6m", "2w"}, 0.67},
        {{"", "6m", "1m"}, 0.67},
        {{"", "6m", "3m"}, 0.85},
        {{"", "6m", "1y"}, 0.86},
        {{"", "6m", "2y"}, 0.76},
        {{"", "6m", "3y"}, 0.69},
        {{"", "6m", "5y"}, 0.59},
        {{"", "6m", "10y"}, 0.47},
        {{"", "6m", "15y"}, 0.4},
        {{"", "6m", "20y"}, 0.37},
        {{"", "6m", "30y"}, 0.32},
        {{"", "1y", "2w"}, 0.53},
        {{"", "1y", "1m"}, 0.53},
        {{"", "1y", "3m"}, 0.69},
        {{"", "1y", "6m"}, 0.86},
        {{"", "1y", "2y"}, 0.93},
        {{"", "1y", "3y"}, 0.87},
        {{"", "1y", "5y"}, 0.77},
        {{"", "1y", "10y"}, 0.63},
        {{"", "1y", "15y"}, 0.57},
        {{"", "1y", "20y"}, 0.54},
        {{"", "1y", "30y"}, 0.5},
        {{"", "2y", "2w"}, 0.42},
        {{"", "2y", "1m"}, 0.42},
        {{"", "2y", "3m"}, 0.57},
        {{"", "2y", "6m"}, 0.76},
        {{"", "2y", "1y"}, 0.93},
        {{"", "2y", "3y"}, 0.98},
        {{"", "2y", "5y"}, 0.9},
        {{"", "2y", "10y"}, 0.77},
        {{"", "2y", "15y"}, 0.7},
        {{"", "2y", "20y"}, 0.67},
        {{"", "2y", "30y"}, 0.63},
        {{"", "3y", "2w"}, 0.37},
        {{"", "3y", "1m"}, 0.37},
        {{"", "3y", "3m"}, 0.5},
        {{"", "3y", "6m"}, 0.69},
        {{"", "3y", "1y"}, 0.87},
        {{"", "3y", "2y"}, 0.98},
        {{"", "3y", "5y"}, 0.96},
        {{"", "3y", "10y"}, 0.84},
        {{"", "3y", "15y"}, 0.78},
        {{"", "3y", "20y"}, 0.75},
        {{"", "3y", "30y"}, 0.71},
        {{"", "5y", "2w"}, 0.3},
        {{"", "5y", "1m"}, 0.3},
        {{"", "5y", "3m"}, 0.42},
        {{"", "5y", "6m"}, 0.59},
        {{"", "5y", "1y"}, 0.77},
        {{"", "5y", "2y"}, 0.9},
        {{"", "5y", "3y"}, 0.96},
        {{"", "5y", "10y"}, 0.93},
        {{"", "5y", "15y"}, 0.89},
        {{"", "5y", "20y"}, 0.86},
        {{"", "5y", "30y"}, 0.82},
        {{"", "10y", "2w"}, 0.22},
        {{"", "10y", "1m"}, 0.22},
        {{"", "10y", "3m"}, 0.32},
        {{"", "10y", "6m"}, 0.47},
        {{"", "10y", "1y"}, 0.63},
        {{"", "10y", "2y"}, 0.77},
        {{"", "10y", "3y"}, 0.84},
        {{"", "10y", "5y"}, 0.93},
        {{"", "10y", "15y"}, 0.98},
        {{"", "10y", "20y"}, 0.96},
        {{"", "10y", "30y"}, 0.94},
        {{"", "15y", "2w"}, 0.18},
        {{"", "15y", "1m"}, 0.18},
        {{"", "15y", "3m"}, 0.25},
        {{"", "15y", "6m"}, 0.4},
        {{"", "15y", "1y"}, 0.57},
        {{"", "15y", "2y"}, 0.7},
        {{"", "15y", "3y"}, 0.78},
        {{"", "15y", "5y"}, 0.89},
        {{"", "15y", "10y"}, 0.98},
        {{"", "15y", "20y"}, 0.99},
        {{"", "15y", "30y"}, 0.98},
        {{"", "20y", "2w"}, 0.16},
        {{"", "20y", "1m"}, 0.16},
        {{"", "20y", "3m"}, 0.23},
        {{"", "20y", "6m"}, 0.37},
        {{"", "20y", "1y"}, 0.54},
        {{"", "20y", "2y"}, 0.67},
        {{"", "20y", "3y"}, 0.75},
        {{"", "20y", "5y"}, 0.86},
        {{"", "20y", "10y"}, 0.96},
        {{"", "20y", "15y"}, 0.99},
        {{"", "20y", "30y"}, 0.99},
        {{"", "30y", "2w"}, 0.12},
        {{"", "30y", "1m"}, 0.12},
        {{"", "30y", "3m"}, 0.2},
        {{"", "30y", "6m"}, 0.32},
        {{"", "30y", "1y"}, 0.5},
        {{"", "30y", "2y"}, 0.63},
        {{"", "30y", "3y"}, 0.71},
        {{"", "30y", "5y"}, 0.82},
        {{"", "30y", "10y"}, 0.94},
        {{"", "30y", "15y"}, 0.98},
        {{"", "30y", "20y"}, 0.99}
    };

    // CreditQ inter-bucket correlations
    interBucketCorrelation_[RiskType::CreditQ] = {
        {{"", "1", "2"}, 0.42},
        {{"", "1", "3"}, 0.39},
        {{"", "1", "4"}, 0.39},
        {{"", "1", "5"}, 0.4},
        {{"", "1", "6"}, 0.38},
        {{"", "1", "7"}, 0.39},
        {{"", "1", "8"}, 0.34},
        {{"", "1", "9"}, 0.37},
        {{"", "1", "10"}, 0.39},
        {{"", "1", "11"}, 0.37},
        {{"", "1", "12"}, 0.31},
        {{"", "2", "1"}, 0.42},
        {{"", "2", "3"}, 0.44},
        {{"", "2", "4"}, 0.45},
        {{"", "2", "5"}, 0.47},
        {{"", "2", "6"}, 0.45},
        {{"", "2", "7"}, 0.33},
        {{"", "2", "8"}, 0.4},
        {{"", "2", "9"}, 0.41},
        {{"", "2", "10"}, 0.44},
        {{"", "2", "11"}, 0.43},
        {{"", "2", "12"}, 0.37},
        {{"", "3", "1"}, 0.39},
        {{"", "3", "2"}, 0.44},
        {{"", "3", "4"}, 0.43},
        {{"", "3", "5"}, 0.45},
        {{"", "3", "6"}, 0.43},
        {{"", "3", "7"}, 0.32},
        {{"", "3", "8"}, 0.35},
        {{"", "3", "9"}, 0.41},
        {{"", "3", "10"}, 0.42},
        {{"", "3", "11"}, 0.4},
        {{"", "3", "12"}, 0.36},
        {{"", "4", "1"}, 0.39},
        {{"", "4", "2"}, 0.45},
        {{"", "4", "3"}, 0.43},
        {{"", "4", "5"}, 0.47},
        {{"", "4", "6"}, 0.44},
        {{"", "4", "7"}, 0.3},
        {{"", "4", "8"}, 0.34},
        {{"", "4", "9"}, 0.39},
        {{"", "4", "10"}, 0.43},
        {{"", "4", "11"}, 0.39},
        {{"", "4", "12"}, 0.36},
        {{"", "5", "1"}, 0.4},
        {{"", "5", "2"}, 0.47},
        {{"", "5", "3"}, 0.45},
        {{"", "5", "4"}, 0.47},
        {{"", "5", "6"}, 0.47},
        {{"", "5", "7"}, 0.31},
        {{"", "5", "8"}, 0.35},
        {{"", "5", "9"}, 0.4},
        {{"", "5", "10"}, 0.44},
        {{"", "5", "11"}, 0.42},
        {{"", "5", "12"}, 0.37},
        {{"", "6", "1"}, 0.38},
        {{"", "6", "2"}, 0.45},
        {{"", "6", "3"}, 0.43},
        {{"", "6", "4"}, 0.44},
        {{"", "6", "5"}, 0.47},
        {{"", "6", "7"}, 0.3},
        {{"", "6", "8"}, 0.34},
        {{"", "6", "9"}, 0.38},
        {{"", "6", "10"}, 0.4},
        {{"", "6", "11"}, 0.39},
        {{"", "6", "12"}, 0.38},
        {{"", "7", "1"}, 0.39},
        {{"", "7", "2"}, 0.33},
        {{"", "7", "3"}, 0.32},
        {{"", "7", "4"}, 0.3},
        {{"", "7", "5"}, 0.31},
        {{"", "7", "6"}, 0.3},
        {{"", "7", "8"}, 0.28},
        {{"", "7", "9"}, 0.31},
        {{"", "7", "10"}, 0.31},
        {{"", "7", "11"}, 0.3},
        {{"", "7", "12"}, 0.26},
        {{"", "8", "1"}, 0.34},
        {{"", "8", "2"}, 0.4},
        {{"", "8", "3"}, 0.35},
        {{"", "8", "4"}, 0.34},
        {{"", "8", "5"}, 0.35},
        {{"", "8", "6"}, 0.34},
        {{"", "8", "7"}, 0.28},
        {{"", "8", "9"}, 0.34},
        {{"", "8", "10"}, 0.35},
        {{"", "8", "11"}, 0.33},
        {{"", "8", "12"}, 0.3},
        {{"", "9", "1"}, 0.37},
        {{"", "9", "2"}, 0.41},
        {{"", "9", "3"}, 0.41},
        {{"", "9", "4"}, 0.39},
        {{"", "9", "5"}, 0.4},
        {{"", "9", "6"}, 0.38},
        {{"", "9", "7"}, 0.31},
        {{"", "9", "8"}, 0.34},
        {{"", "9", "10"}, 0.4},
        {{"", "9", "11"}, 0.37},
        {{"", "9", "12"}, 0.32},
        {{"", "10", "1"}, 0.39},
        {{"", "10", "2"}, 0.44},
        {{"", "10", "3"}, 0.42},
        {{"", "10", "4"}, 0.43},
        {{"", "10", "5"}, 0.44},
        {{"", "10", "6"}, 0.4},
        {{"", "10", "7"}, 0.31},
        {{"", "10", "8"}, 0.35},
        {{"", "10", "9"}, 0.4},
        {{"", "10", "11"}, 0.4},
        {{"", "10", "12"}, 0.35},
        {{"", "11", "1"}, 0.37},
        {{"", "11", "2"}, 0.43},
        {{"", "11", "3"}, 0.4},
        {{"", "11", "4"}, 0.39},
        {{"", "11", "5"}, 0.42},
        {{"", "11", "6"}, 0.39},
        {{"", "11", "7"}, 0.3},
        {{"", "11", "8"}, 0.33},
        {{"", "11", "9"}, 0.37},
        {{"", "11", "10"}, 0.4},
        {{"", "11", "12"}, 0.34},
        {{"", "12", "1"}, 0.31},
        {{"", "12", "2"}, 0.37},
        {{"", "12", "3"}, 0.36},
        {{"", "12", "4"}, 0.36},
        {{"", "12", "5"}, 0.37},
        {{"", "12", "6"}, 0.38},
        {{"", "12", "7"}, 0.26},
        {{"", "12", "8"}, 0.3},
        {{"", "12", "9"}, 0.32},
        {{"", "12", "10"}, 0.35},
        {{"", "12", "11"}, 0.34}
    };

    // Equity inter-bucket correlations
    interBucketCorrelation_[RiskType::Equity] = {
        {{"", "1", "2"}, 0.15},
        {{"", "1", "3"}, 0.14},
        {{"", "1", "4"}, 0.16},
        {{"", "1", "5"}, 0.1},
        {{"", "1", "6"}, 0.12},
        {{"", "1", "7"}, 0.1},
        {{"", "1", "8"}, 0.11},
        {{"", "1", "9"}, 0.13},
        {{"", "1", "10"}, 0.09},
        {{"", "1", "11"}, 0.17},
        {{"", "1", "12"}, 0.17},
        {{"", "2", "1"}, 0.15},
        {{"", "2", "3"}, 0.16},
        {{"", "2", "4"}, 0.17},
        {{"", "2", "5"}, 0.1},
        {{"", "2", "6"}, 0.11},
        {{"", "2", "7"}, 0.1},
        {{"", "2", "8"}, 0.11},
        {{"", "2", "9"}, 0.14},
        {{"", "2", "10"}, 0.09},
        {{"", "2", "11"}, 0.17},
        {{"", "2", "12"}, 0.17},
        {{"", "3", "1"}, 0.14},
        {{"", "3", "2"}, 0.16},
        {{"", "3", "4"}, 0.19},
        {{"", "3", "5"}, 0.14},
        {{"", "3", "6"}, 0.17},
        {{"", "3", "7"}, 0.18},
        {{"", "3", "8"}, 0.17},
        {{"", "3", "9"}, 0.16},
        {{"", "3", "10"}, 0.14},
        {{"", "3", "11"}, 0.25},
        {{"", "3", "12"}, 0.25},
        {{"", "4", "1"}, 0.16},
        {{"", "4", "2"}, 0.17},
        {{"", "4", "3"}, 0.19},
        {{"", "4", "5"}, 0.15},
        {{"", "4", "6"}, 0.18},
        {{"", "4", "7"}, 0.18},
        {{"", "4", "8"}, 0.18},
        {{"", "4", "9"}, 0.18},
        {{"", "4", "10"}, 0.14},
        {{"", "4", "11"}, 0.28},
        {{"", "4", "12"}, 0.28},
        {{"", "5", "1"}, 0.1},
        {{"", "5", "2"}, 0.1},
        {{"", "5", "3"}, 0.14},
        {{"", "5", "4"}, 0.15},
        {{"", "5", "6"}, 0.28},
        {{"", "5", "7"}, 0.23},
        {{"", "5", "8"}, 0.27},
        {{"", "5", "9"}, 0.13},
        {{"", "5", "10"}, 0.21},
        {{"", "5", "11"}, 0.35},
        {{"", "5", "12"}, 0.35},
        {{"", "6", "1"}, 0.12},
        {{"", "6", "2"}, 0.11},
        {{"", "6", "3"}, 0.17},
        {{"", "6", "4"}, 0.18},
        {{"", "6", "5"}, 0.28},
        {{"", "6", "7"}, 0.3},
        {{"", "6", "8"}, 0.34},
        {{"", "6", "9"}, 0.16},
        {{"", "6", "10"}, 0.26},
        {{"", "6", "11"}, 0.45},
        {{"", "6", "12"}, 0.45},
        {{"", "7", "1"}, 0.1},
        {{"", "7", "2"}, 0.1},
        {{"", "7", "3"}, 0.18},
        {{"", "7", "4"}, 0.18},
        {{"", "7", "5"}, 0.23},
        {{"", "7", "6"}, 0.3},
        {{"", "7", "8"}, 0.29},
        {{"", "7", "9"}, 0.15},
        {{"", "7", "10"}, 0.24},
        {{"", "7", "11"}, 0.41},
        {{"", "7", "12"}, 0.41},
        {{"", "8", "1"}, 0.11},
        {{"", "8", "2"}, 0.11},
        {{"", "8", "3"}, 0.17},
        {{"", "8", "4"}, 0.18},
        {{"", "8", "5"}, 0.27},
        {{"", "8", "6"}, 0.34},
        {{"", "8", "7"}, 0.29},
        {{"", "8", "9"}, 0.16},
        {{"", "8", "10"}, 0.26},
        {{"", "8", "11"}, 0.44},
        {{"", "8", "12"}, 0.44},
        {{"", "9", "1"}, 0.13},
        {{"", "9", "2"}, 0.14},
        {{"", "9", "3"}, 0.16},
        {{"", "9", "4"}, 0.18},
        {{"", "9", "5"}, 0.13},
        {{"", "9", "6"}, 0.16},
        {{"", "9", "7"}, 0.15},
        {{"", "9", "8"}, 0.16},
        {{"", "9", "10"}, 0.13},
        {{"", "9", "11"}, 0.24},
        {{"", "9", "12"}, 0.24},
        {{"", "10", "1"}, 0.09},
        {{"", "10", "2"}, 0.09},
        {{"", "10", "3"}, 0.14},
        {{"", "10", "4"}, 0.14},
        {{"", "10", "5"}, 0.21},
        {{"", "10", "6"}, 0.26},
        {{"", "10", "7"}, 0.24},
        {{"", "10", "8"}, 0.26},
        {{"", "10", "9"}, 0.13},
        {{"", "10", "11"}, 0.33},
        {{"", "10", "12"}, 0.33},
        {{"", "11", "1"}, 0.17},
        {{"", "11", "2"}, 0.17},
        {{"", "11", "3"}, 0.25},
        {{"", "11", "4"}, 0.28},
        {{"", "11", "5"}, 0.35},
        {{"", "11", "6"}, 0.45},
        {{"", "11", "7"}, 0.41},
        {{"", "11", "8"}, 0.44},
        {{"", "11", "9"}, 0.24},
        {{"", "11", "10"}, 0.33},
        {{"", "11", "12"}, 0.62},
        {{"", "12", "1"}, 0.17},
        {{"", "12", "2"}, 0.17},
        {{"", "12", "3"}, 0.25},
        {{"", "12", "4"}, 0.28},
        {{"", "12", "5"}, 0.35},
        {{"", "12", "6"}, 0.45},
        {{"", "12", "7"}, 0.41},
        {{"", "12", "8"}, 0.44},
        {{"", "12", "9"}, 0.24},
        {{"", "12", "10"}, 0.33},
        {{"", "12", "11"}, 0.62}
    };

    // Commodity inter-bucket correlations
    interBucketCorrelation_[RiskType::Commodity] = {
        {{"", "1", "2"}, 0.18},
        {{"", "1", "3"}, 0.15},
        {{"", "1", "4"}, 0.2},
        {{"", "1", "5"}, 0.25},
        {{"", "1", "6"}, 0.08},
        {{"", "1", "7"}, 0.19},
        {{"", "1", "8"}, 0.01},
        {{"", "1", "9"}, 0.27},
        {{"", "1", "10"}, 0.0},
        {{"", "1", "11"}, 0.15},
        {{"", "1", "12"}, 0.02},
        {{"", "1", "13"}, 0.06},
        {{"", "1", "14"}, 0.07},
        {{"", "1", "15"}, -0.04},
        {{"", "1", "16"}, 0.0},
        {{"", "1", "17"}, 0.06},
        {{"", "2", "1"}, 0.18},
        {{"", "2", "3"}, 0.89},
        {{"", "2", "4"}, 0.94},
        {{"", "2", "5"}, 0.93},
        {{"", "2", "6"}, 0.32},
        {{"", "2", "7"}, 0.22},
        {{"", "2", "8"}, 0.27},
        {{"", "2", "9"}, 0.24},
        {{"", "2", "10"}, 0.09},
        {{"", "2", "11"}, 0.45},
        {{"", "2", "12"}, 0.21},
        {{"", "2", "13"}, 0.32},
        {{"", "2", "14"}, 0.28},
        {{"", "2", "15"}, 0.17},
        {{"", "2", "16"}, 0.0},
        {{"", "2", "17"}, 0.37},
        {{"", "3", "1"}, 0.15},
        {{"", "3", "2"}, 0.89},
        {{"", "3", "4"}, 0.87},
        {{"", "3", "5"}, 0.88},
        {{"", "3", "6"}, 0.25},
        {{"", "3", "7"}, 0.16},
        {{"", "3", "8"}, 0.19},
        {{"", "3", "9"}, 0.12},
        {{"", "3", "10"}, 0.1},
        {{"", "3", "11"}, 0.26},
        {{"", "3", "12"}, -0.01},
        {{"", "3", "13"}, 0.19},
        {{"", "3", "14"}, 0.17},
        {{"", "3", "15"}, 0.1},
        {{"", "3", "16"}, 0.0},
        {{"", "3", "17"}, 0.27},
        {{"", "4", "1"}, 0.2},
        {{"", "4", "2"}, 0.94},
        {{"", "4", "3"}, 0.87},
        {{"", "4", "5"}, 0.92},
        {{"", "4", "6"}, 0.29},
        {{"", "4", "7"}, 0.22},
        {{"", "4", "8"}, 0.26},
        {{"", "4", "9"}, 0.19},
        {{"", "4", "10"}, 0.0},
        {{"", "4", "11"}, 0.32},
        {{"", "4", "12"}, 0.05},
        {{"", "4", "13"}, 0.2},
        {{"", "4", "14"}, 0.22},
        {{"", "4", "15"}, 0.13},
        {{"", "4", "16"}, 0.0},
        {{"", "4", "17"}, 0.28},
        {{"", "5", "1"}, 0.25},
        {{"", "5", "2"}, 0.93},
        {{"", "5", "3"}, 0.88},
        {{"", "5", "4"}, 0.92},
        {{"", "5", "6"}, 0.3},
        {{"", "5", "7"}, 0.26},
        {{"", "5", "8"}, 0.22},
        {{"", "5", "9"}, 0.28},
        {{"", "5", "10"}, 0.12},
        {{"", "5", "11"}, 0.42},
        {{"", "5", "12"}, 0.23},
        {{"", "5", "13"}, 0.28},
        {{"", "5", "14"}, 0.29},
        {{"", "5", "15"}, 0.17},
        {{"", "5", "16"}, 0.0},
        {{"", "5", "17"}, 0.34},
        {{"", "6", "1"}, 0.08},
        {{"", "6", "2"}, 0.32},
        {{"", "6", "3"}, 0.25},
        {{"", "6", "4"}, 0.29},
        {{"", "6", "5"}, 0.3},
        {{"", "6", "7"}, 0.13},
        {{"", "6", "8"}, 0.57},
        {{"", "6", "9"}, 0.05},
        {{"", "6", "10"}, 0.14},
        {{"", "6", "11"}, 0.15},
        {{"", "6", "12"}, -0.02},
        {{"", "6", "13"}, 0.13},
        {{"", "6", "14"}, 0.17},
        {{"", "6", "15"}, 0.01},
        {{"", "6", "16"}, 0.0},
        {{"", "6", "17"}, 0.26},
        {{"", "7", "1"}, 0.19},
        {{"", "7", "2"}, 0.22},
        {{"", "7", "3"}, 0.16},
        {{"", "7", "4"}, 0.22},
        {{"", "7", "5"}, 0.26},
        {{"", "7", "6"}, 0.13},
        {{"", "7", "8"}, 0.07},
        {{"", "7", "9"}, 0.8},
        {{"", "7", "10"}, 0.19},
        {{"", "7", "11"}, 0.16},
        {{"", "7", "12"}, 0.05},
        {{"", "7", "13"}, 0.17},
        {{"", "7", "14"}, 0.18},
        {{"", "7", "15"}, 0.0},
        {{"", "7", "16"}, 0.0},
        {{"", "7", "17"}, 0.18},
        {{"", "8", "1"}, 0.01},
        {{"", "8", "2"}, 0.27},
        {{"", "8", "3"}, 0.19},
        {{"", "8", "4"}, 0.26},
        {{"", "8", "5"}, 0.22},
        {{"", "8", "6"}, 0.57},
        {{"", "8", "7"}, 0.07},
        {{"", "8", "9"}, 0.13},
        {{"", "8", "10"}, 0.06},
        {{"", "8", "11"}, 0.16},
        {{"", "8", "12"}, 0.03},
        {{"", "8", "13"}, 0.1},
        {{"", "8", "14"}, 0.12},
        {{"", "8", "15"}, 0.06},
        {{"", "8", "16"}, 0.0},
        {{"", "8", "17"}, 0.23},
        {{"", "9", "1"}, 0.27},
        {{"", "9", "2"}, 0.24},
        {{"", "9", "3"}, 0.12},
        {{"", "9", "4"}, 0.19},
        {{"", "9", "5"}, 0.28},
        {{"", "9", "6"}, 0.05},
        {{"", "9", "7"}, 0.8},
        {{"", "9", "8"}, 0.13},
        {{"", "9", "10"}, 0.15},
        {{"", "9", "11"}, 0.17},
        {{"", "9", "12"}, 0.05},
        {{"", "9", "13"}, 0.15},
        {{"", "9", "14"}, 0.13},
        {{"", "9", "15"}, -0.03},
        {{"", "9", "16"}, 0.0},
        {{"", "9", "17"}, 0.13},
        {{"", "10", "1"}, 0.0},
        {{"", "10", "2"}, 0.09},
        {{"", "10", "3"}, 0.1},
        {{"", "10", "4"}, 0.0},
        {{"", "10", "5"}, 0.12},
        {{"", "10", "6"}, 0.14},
        {{"", "10", "7"}, 0.19},
        {{"", "10", "8"}, 0.06},
        {{"", "10", "9"}, 0.15},
        {{"", "10", "11"}, 0.07},
        {{"", "10", "12"}, 0.07},
        {{"", "10", "13"}, 0.17},
        {{"", "10", "14"}, 0.1},
        {{"", "10", "15"}, 0.02},
        {{"", "10", "16"}, 0.0},
        {{"", "10", "17"}, 0.11},
        {{"", "11", "1"}, 0.15},
        {{"", "11", "2"}, 0.45},
        {{"", "11", "3"}, 0.26},
        {{"", "11", "4"}, 0.32},
        {{"", "11", "5"}, 0.42},
        {{"", "11", "6"}, 0.15},
        {{"", "11", "7"}, 0.16},
        {{"", "11", "8"}, 0.16},
        {{"", "11", "9"}, 0.17},
        {{"", "11", "10"}, 0.07},
        {{"", "11", "12"}, 0.34},
        {{"", "11", "13"}, 0.2},
        {{"", "11", "14"}, 0.21},
        {{"", "11", "15"}, 0.16},
        {{"", "11", "16"}, 0.0},
        {{"", "11", "17"}, 0.27},
        {{"", "12", "1"}, 0.02},
        {{"", "12", "2"}, 0.21},
        {{"", "12", "3"}, -0.01},
        {{"", "12", "4"}, 0.05},
        {{"", "12", "5"}, 0.23},
        {{"", "12", "6"}, -0.02},
        {{"", "12", "7"}, 0.05},
        {{"", "12", "8"}, 0.03},
        {{"", "12", "9"}, 0.05},
        {{"", "12", "10"}, 0.07},
        {{"", "12", "11"}, 0.34},
        {{"", "12", "13"}, 0.17},
        {{"", "12", "14"}, 0.26},
        {{"", "12", "15"}, 0.11},
        {{"", "12", "16"}, 0.0},
        {{"", "12", "17"}, 0.14},
        {{"", "13", "1"}, 0.06},
        {{"", "13", "2"}, 0.32},
        {{"", "13", "3"}, 0.19},
        {{"", "13", "4"}, 0.2},
        {{"", "13", "5"}, 0.28},
        {{"", "13", "6"}, 0.13},
        {{"", "13", "7"}, 0.17},
        {{"", "13", "8"}, 0.1},
        {{"", "13", "9"}, 0.15},
        {{"", "13", "10"}, 0.17},
        {{"", "13", "11"}, 0.2},
        {{"", "13", "12"}, 0.17},
        {{"", "13", "14"}, 0.35},
        {{"", "13", "15"}, 0.09},
        {{"", "13", "16"}, 0.0},
        {{"", "13", "17"}, 0.22},
        {{"", "14", "1"}, 0.07},
        {{"", "14", "2"}, 0.28},
        {{"", "14", "3"}, 0.17},
        {{"", "14", "4"}, 0.22},
        {{"", "14", "5"}, 0.29},
        {{"", "14", "6"}, 0.17},
        {{"", "14", "7"}, 0.18},
        {{"", "14", "8"}, 0.12},
        {{"", "14", "9"}, 0.13},
        {{"", "14", "10"}, 0.1},
        {{"", "14", "11"}, 0.21},
        {{"", "14", "12"}, 0.26},
        {{"", "14", "13"}, 0.35},
        {{"", "14", "15"}, 0.06},
        {{"", "14", "16"}, 0.0},
        {{"", "14", "17"}, 0.2},
        {{"", "15", "1"}, -0.04},
        {{"", "15", "2"}, 0.17},
        {{"", "15", "3"}, 0.1},
        {{"", "15", "4"}, 0.13},
        {{"", "15", "5"}, 0.17},
        {{"", "15", "6"}, 0.01},
        {{"", "15", "7"}, 0.0},
        {{"", "15", "8"}, 0.06},
        {{"", "15", "9"}, -0.03},
        {{"", "15", "10"}, 0.02},
        {{"", "15", "11"}, 0.16},
        {{"", "15", "12"}, 0.11},
        {{"", "15", "13"}, 0.09},
        {{"", "15", "14"}, 0.06},
        {{"", "15", "16"}, 0.0},
        {{"", "15", "17"}, 0.16},
        {{"", "16", "1"}, 0.0},
        {{"", "16", "2"}, 0.0},
        {{"", "16", "3"}, 0.0},
        {{"", "16", "4"}, 0.0},
        {{"", "16", "5"}, 0.0},
        {{"", "16", "6"}, 0.0},
        {{"", "16", "7"}, 0.0},
        {{"", "16", "8"}, 0.0},
        {{"", "16", "9"}, 0.0},
        {{"", "16", "10"}, 0.0},
        {{"", "16", "11"}, 0.0},
        {{"", "16", "12"}, 0.0},
        {{"", "16", "13"}, 0.0},
        {{"", "16", "14"}, 0.0},
        {{"", "16", "15"}, 0.0},
        {{"", "16", "17"}, 0.0},
        {{"", "17", "1"}, 0.06},
        {{"", "17", "2"}, 0.37},
        {{"", "17", "3"}, 0.27},
        {{"", "17", "4"}, 0.28},
        {{"", "17", "5"}, 0.34},
        {{"", "17", "6"}, 0.26},
        {{"", "17", "7"}, 0.18},
        {{"", "17", "8"}, 0.23},
        {{"", "17", "9"}, 0.13},
        {{"", "17", "10"}, 0.11},
        {{"", "17", "11"}, 0.27},
        {{"", "17", "12"}, 0.14},
        {{"", "17", "13"}, 0.22},
        {{"", "17", "14"}, 0.2},
        {{"", "17", "15"}, 0.16},
        {{"", "17", "16"}, 0.0}
    };

    // Equity intra-bucket correlations (exclude Residual and deal with it in the method - it is 0%)
    intraBucketCorrelation_[RiskType::Equity] = {
        {{"1", "", ""}, 0.14},
        {{"2", "", ""}, 0.20},
        {{"3", "", ""}, 0.19},
        {{"4", "", ""}, 0.21},
        {{"5", "", ""}, 0.24},
        {{"6", "", ""}, 0.35},
        {{"7", "", ""}, 0.34},
        {{"8", "", ""}, 0.34},
        {{"9", "", ""}, 0.20},
        {{"10", "", ""}, 0.24},
        {{"11", "", ""}, 0.62},
        {{"12", "", ""}, 0.62}
    };

    // Commodity intra-bucket correlations
    intraBucketCorrelation_[RiskType::Commodity] = {
        {{"1", "", ""}, 0.30},
        {{"2", "", ""}, 0.97},
        {{"3", "", ""}, 0.93},
        {{"4", "", ""}, 0.98},
        {{"5", "", ""}, 0.99},
        {{"6", "", ""}, 0.92},
        {{"7", "", ""}, 1.00},
        {{"8", "", ""}, 0.58},
        {{"9", "", ""}, 1.00},
        {{"10", "", ""}, 0.10},
        {{"11", "", ""}, 0.55},
        {{"12", "", ""}, 0.64},
        {{"13", "", ""}, 0.71},
        {{"14", "", ""}, 0.22},
        {{"15", "", ""}, 0.29},
        {{"16", "", ""}, 0.00},
        {{"17", "", ""}, 0.21}
    };

    // Initialise the single, ad-hoc type, correlations
    xccyCorr_ = 0.20;
    infCorr_ = 0.29;
    infVolCorr_ = 0.29;
    irSubCurveCorr_ = 0.98;
    irInterCurrencyCorr_ = 0.23;
    crqResidualIntraCorr_ = 0.5;
    crqSameIntraCorr_ = 0.97;
    crqDiffIntraCorr_ = 0.45;
    crnqResidualIntraCorr_ = 0.5;
    crnqSameIntraCorr_ = 0.57;
    crnqDiffIntraCorr_ = 0.27;
    crnqInterCorr_ = 0.21;
    fxCorr_ = 0.5;
    basecorrCorr_ = 0.10;

    // clang-format on
}

void SimmConfiguration_ISDA_V2_0::addLabels2(const RiskType& rt, const string& label_2) {
    // Call the shared implementation
    SimmConfigurationBase::addLabels2Impl(rt, label_2);
}

string SimmConfiguration_ISDA_V2_0::label2(const QuantLib::ext::shared_ptr<InterestRateIndex>& irIndex) const {
    // Special for BMA
    if (boost::algorithm::starts_with(irIndex->name(), "BMA")) {
        return "Municipal";
    }

    // Otherwise pass off to base class
    return SimmConfigurationBase::label2(irIndex);
}

} // namespace analytics
} // namespace ore
