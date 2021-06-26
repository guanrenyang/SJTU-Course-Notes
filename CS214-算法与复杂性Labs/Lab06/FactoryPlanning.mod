/*********************************************
 * OPL 12.9.0.0 Model
 * Author: 69540
 * Creation Date: 2021年4月7日 at 下午7:38:17
 *********************************************/
int NbMonths = ...;

{string} Prod = ...;
{string} Process = ...;
range Months=1..NbMonths;
// profitProd[j] is profit per unit for product j
float ProfitProd[Prod] = ...;

// processProd[i][j] gives hours of process i required by product j
float ProcessProd[Process][Prod] = ...;
// marketProd[i][j] gives marketing limitation on product j for month i
float MarketProd[Months][Prod] = ...;

float CostHold  = ...;
float StartHold = ...;
float EndHold   = ...;
float MaxHold   = ...;

// process capacity
float HoursMonth = ...; // 2 eight hour shifts per day, 24 working days per month;

// number of each type of machine
int NumProcess[Process] = ...;

// how many machines must be down over 6 month period
int NumDown[Process] = ...;

dvar float+ Produce[Months][Prod];
dvar float+ Hold[0..NbMonths][Prod] in 0..MaxHold;
dvar float+ Sell[m in Months][j in Prod] in 0..MarketProd[m][j];
dvar int+   Down[m in Months][i in Process] in 0..NumProcess[i];

dexpr float Profit = 
  sum (j in Prod, m in Months) ProfitProd[j] * Sell[m][j];
dexpr float Cost = 
  sum (j in Prod, m in Months) CostHold * Hold[m][j];
dexpr float NetProfit =
	Profit-Cost;

maximize NetProfit;

subject to{
	forall(m in Months, i in Process)
	  constraintProcess: sum(j in Prod) ProcessProd[i][j]*Produce[m][j]+HoursMonth*Down[m][i]<=NumProcess[i]*HoursMonth;
	
	forall(m in Months, j in Prod)
	  constraintHold: Hold[m][j]==Hold[m-1][j]+Produce[m][j]-Sell[m][j];
	  	
	forall(i in Process)
	  constraintDown: sum(m in Months) Down[m][i]>=NumDown[i];
	  
  	forall(j in Prod)
  	  {
  	    constraintStartHold: Hold[0][j]==StartHold;
  	  	constraintEndHold: Hold[NbMonths][j]==EndHold;	  
  	  }
}

