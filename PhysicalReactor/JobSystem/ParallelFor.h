#pragma once

#include "JobScheduler.h"
#include <functional>
//#include "Delegates/Delegate.h"



namespace PRE {
	
	template <typename T, typename S>
	struct parallel_for_job_data
	{
		typedef T DataType;
		typedef S SplitterType;

		//typedef void(*Function)(DataType*, uint32_t, void*);
		
		parallel_for_job_data(DataType* data, uint32_t count, std::function<void(DataType*, uint32_t, void*)>  f, void* extraData, const SplitterType& splitter)
			: data(data)
			, count(count)
			, function(f)
			, splitter(splitter)
			, extraData(extraData)
		{
		}

		DataType* data;
		void* extraData;
		uint32_t count;
		std::function<void(DataType*, uint32_t, void*)>  function;
		SplitterType splitter;
	};

	template <typename JobData>
	void parallel_for_job(Job* job, const void* jobData)
	{
        typedef typename JobData::DataType DataType;
        typedef typename JobData::SplitterType SplitterType;

		const JobData* data = static_cast<const JobData*>(jobData);
        const SplitterType& splitter = data->splitter;
		void* extraData = data->extraData;

        if (splitter.Split<DataType>(data->count))
		{
			// split in two
			const uint32_t leftCount = data->count / 2u;
			const JobData leftData(data->data, leftCount,data->function, extraData, splitter);
			Job* left = JobScheduler::CreateChildJob(job, &PRE::parallel_for_job<JobData>, leftData);
			JobScheduler::Run(left);

			const uint32_t rightCount = data->count - leftCount;
			const JobData rightData(data->data + leftCount, rightCount, data->function, extraData, splitter);
			Job* right = JobScheduler::CreateChildJob(job, &PRE::parallel_for_job<JobData>, rightData);
			JobScheduler::Run(right);
		}
		else
		{
			// execute the function on the range of data
			(data->function)(data->data, data->count, extraData);
		}
	}
}

class CountSplitter
{
public:
	explicit CountSplitter(uint32_t count = 50u)
		: m_count(count)
	{
	}

	template <typename T>
	inline bool Split(uint32_t count) const
	{
		return (count > m_count);
	}

private:
	uint32_t m_count;
};

class DataSizeSplitter
{
public:
	explicit DataSizeSplitter(uint32_t size)
		: m_size(size)
	{
	}

	template <typename T>
	inline bool Split(uint32_t count) const
	{
		return (count*sizeof(T) > m_size);
	}

private:
	uint32_t m_size;
};

template<typename T, typename S = CountSplitter>
Job* parallel_for(T* data, uint32_t count, std::function<void(T*, uint32_t, void*)> function ,void* extraData = nullptr, const S& splitter = S()) {
	typedef PRE::parallel_for_job_data<T, S> JobData;
	
	 JobData jobData(data, count,function, extraData, splitter);

	Job* job = JobScheduler::CreateJob(&PRE::parallel_for_job<JobData>, jobData);

	JobScheduler::Run(job);
	return job;
}


