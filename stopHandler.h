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



	inline static float stopTimePowerMult = 1;

	inline static float stopTimeHandToHandMult = 1;
	inline static float stopTimeDaggerMult = 1;
	inline static float stopTimeSwordMult = 1;
	inline static float stopTimeGreatSwordMult = 1;
	inline static float stopTimeAxeMult = 1;
	inline static float stopTimeMaceMult = 1;
	inline static float stopTime2hwMult = 1;
}


namespace stopSpeed 
{
	inline static float objectStopSpeedPercent = 0.1;
	inline static float bashStopSpeedPercent = 0.1;
	inline static float blockedStopSpeedPercent = 0.1;
	inline static float creatureStopSpeedPercent = 0.1;



	inline static float stopSpeedPowerMult = 1;

	inline static float stopSpeedHandToHandMult = 1;
	inline static float stopSpeedDaggerMult = 1;
	inline static float stopSpeedSwordMult = 1;
	inline static float stopSpeedGreatSwordMult = 1;
	inline static float stopSpeedAxeMult = 1;
	inline static float stopSpeedMaceMult = 1;
	inline static float stopSpeed2hwMult = 1;

}
