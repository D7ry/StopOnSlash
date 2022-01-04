#pragma once
class stopHandler
{
public:
	static void refreshVal();
	static void objectStop(bool isPowerAtk);
	static void bashStop(bool isPowerAtk);
	static void blockedStop(bool isPowerAtk);
	static void creatureStop(bool isPowerAtk);
	
private:
	static void powerMult(float stopTime, float stopSpeed, bool isPower);
	static float getTimeMult();
	static float getSpeedMult();
};

namespace stopTimeMs
{
	inline static float objectStopTime = 100;
	inline static float bashStopTime = 100;
	inline static float blockedStopTime = 100;
	inline static float creatureStopTime = 100;



	inline static float stopTimePowerMult = 100;

	inline static float stopTimeHandToHandMult = 100;
	inline static float stopTimeDaggerMult = 100;
	inline static float stopTimeSwordMult = 100;
	inline static float stopTimeGreatSwordMult = 100;
	inline static float stopTimeAxeMult = 100;
	inline static float stopTimeMaceMult = 100;
	inline static float stopTime2hwMult = 100;
}


namespace stopSpeed 
{
	inline static float objectStopSpeedPercent = 10;
	inline static float bashStopSpeedPercent = 10;
	inline static float blockedStopSpeedPercent = 10;
	inline static float creatureStopSpeedPercent = 10;



	inline static float stopSpeedPowerMult = 100;

	inline static float stopSpeedHandToHandMult = 100;
	inline static float stopSpeedDaggerMult = 100;
	inline static float stopSpeedSwordMult = 100;
	inline static float stopSpeedGreatSwordMult = 100;
	inline static float stopSpeedAxeMult = 100;
	inline static float stopSpeedMaceMult = 100;
	inline static float stopSpeed2hwMult = 100;

}
