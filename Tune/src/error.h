#pragma once

#include "eval_params.h"
#include "pos.h"
#include <span>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <cmath>

inline double sigmoid(double s, double k)
{
	return 1.0 / (1.0 + std::exp(-k * s / 400.0));
}

class ErrorThread {
public:
	ErrorThread();
	~ErrorThread() = default;

	struct Result
	{
	public:
		Result(ErrorThread& thread);
		Result(const Result&) = default;
		Result& operator=(const Result&) = default;
		double waitForResult();
	private:
		ErrorThread* m_Thread;
	};

	Result process(const std::span<const Position>& positions, const EvalParams& params, double kValue);
	void quit();
private:
	void wait();

	friend Result;

	std::mutex m_Mutex;
	std::condition_variable m_Condition;
	bool m_IsReady;
	bool m_Quit;
	bool m_Finished;
	double m_Result;

	std::span<const Position> m_Positions;
	EvalParams m_Params;
	double m_kValue;
	std::jthread m_Thread;
};

double errorRaw(const std::span<const Position>& positions, const EvalParams& params, double kValue);
double error(const std::vector<Position>& positions, const EvalParams& params, double kValue, std::vector<ErrorThread>& threads);