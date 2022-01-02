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
	static void powerMult(int stopTime, float stopSpeed, bool isPower);
	static float getTimeMult();
	static float getSpeedMult();
};

namespace stopTimeMs
{
	inline static float objectStopTime;
	inline static float bashStopTime;
	inline static float blockedStopTime;
	inline static float creatureStopTime;

	inline static float stopTimePowerMult;

	inline static float stopTimeHandToHandMult;
	inline static float stopTimeDaggerMult;
	inline static float stopTimeSwordMult;
	inline static float stopTimeGreatSwordMult;
	inline static float stopTimeAxeMult;
	inline static float stopTimeMaceMult;
	inline static float stopTime2hwMult;
}


namespace stopSpeed 
{
	inline static float objectStopSpeedPercent;
	inline static float bashStopSpeedPercent;
	inline static float blockedStopSpeedPercent;
	inline static float creatureStopSpeedPercent;

	inline static float stopSpeedPowerMult;

	inline static float stopSpeedHandToHandMult;
	inline static float stopSpeedDaggerMult;
	inline static float stopSpeedSwordMult;
	inline static float stopSpeedGreatSwordMult;
	inline static float stopSpeedAxeMult;
	inline static float stopSpeedMaceMult;
	inline static float stopSpeed2hwMult;

}