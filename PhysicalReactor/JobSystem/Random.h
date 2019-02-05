
#pragma once

#include <random>


struct Distribution {
	enum T{
		Uniform = 0,
		Bernoulli = 1,
		Binomial = 2,
		Geometric = 3,
		NegativeBinomial,
		Poisson,
		Exponential,
		Gamma,
		Weibull,
		ExtremeValue,
		Normal,
		LogNormal,
		ChiSquared,
		Cauchy,
		Fisher,
		Student,
		Discrete,
		PiecewiseConstant,
		PiecewiseLinear
	};
};

struct Generator {
	enum T {
		MinimalStandard,
		MinimalStandard0,
		MersenneTwister19937
	};
};


namespace detail {
	template<typename T, Distribution::T dst, bool>
	struct DistributionSelector {};

	//Uniform
	template<typename T> struct DistributionSelector<T,Distribution::Uniform, true> { 
		typedef std::uniform_int_distribution<T> distribution_type; 
	};
	template<typename T> struct DistributionSelector<T,Distribution::Uniform, false> { 
		typedef std::uniform_real_distribution<T> distribution_type; 
	};

	//Bernoulli
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Bernoulli,b> { 
		typedef std::bernoulli_distribution distribution_type; 
	};

	//Binomial
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Binomial, b> { 
		static_assert(b, "Binomial distrubution for non-integer types is impossible");
		typedef std::binomial_distribution<T> distribution_type;
	};

	//Geometric / Exponential
	template<typename T> struct DistributionSelector<T,Distribution::Geometric, true> {
		typedef std::geometric_distribution<T> distribution_type;
	};
	template<typename T> struct DistributionSelector<T,Distribution::Geometric, false> {
		typedef std::exponential_distribution<T> distribution_type;
	};
	template<typename T> struct DistributionSelector<T,Distribution::Exponential, true> {
		typedef std::geometric_distribution<T> distribution_type;
	};
	template<typename T> struct DistributionSelector<T,Distribution::Exponential, false> {
		typedef std::exponential_distribution<T> distribution_type;
	};

	//Negative binomial
	template<typename T, bool b> struct DistributionSelector<T,Distribution::NegativeBinomial, b> {
		static_assert(b, "Negative Binomial distrubution for non-integer types is impossible");
		typedef std::negative_binomial_distribution<T> distribution_type;
	};

	//Poisson distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Poisson, b> {
		static_assert(b, "Poisson Distrubtion for non-integer types is impossible");
		typedef std::poisson_distribution<T> distribution_type;
	};

	//Gamma distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Gamma, b> {
		static_assert(!b, "Poisson Distribution for non-floating point types is impossible");
		typedef std::gamma_distribution<T> distribution_type;
	};

	//Weibull distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Weibull, b> {
		static_assert(!b, "Weibull Distribution for non-floating point types is impossible");
		typedef std::weibull_distribution<T> distribution_type;
	};

	//ExtremeValue distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::ExtremeValue, b> {
		static_assert(!b, "Extreme Distribution for non-floating point types is impossible");
		typedef std::extreme_value_distribution<T> distribution_type;
	};

	//Normal distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Normal, b> {
		static_assert(!b, "Normal Distribution for non-floating point types is impossible");
		typedef std::normal_distribution<T> distribution_type;
	};

	//lognormal distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::LogNormal, b> {
		static_assert(!b, "Lognormal Distribution for non-floating point types is impossible");
		typedef std::lognormal_distribution<T> distribution_type;
	};

	//Chi-squared distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::ChiSquared, b> {
		static_assert(!b, "Chisquared Distribution for non-floating point types is impossible");
		typedef std::chi_squared_distribution<T> distribution_type;
	};

	//Cauchy distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Cauchy, b> {
		static_assert(!b, "Cauchy Distribution for non-floating point types is impossible");
		typedef std::cauchy_distribution<T> distribution_type;
	};

	//Fisher distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Fisher, b> {
		static_assert(!b, "Fisher Distribution for non-floating point types is impossible");
		typedef std::fisher_f_distribution<T> distribution_type;
	};

	//Student-t distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Student, b> {
		static_assert(!b, "Student Distribution for non-floating point types is impossible");
		typedef std::student_t_distribution<T> distribution_type;
	};

	//Discrete distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::Discrete, b> {
		static_assert(b, "Discrete Distribution for non-integer types is impossible");
		typedef std::discrete_distribution<T> distribution_type;
	};

	//Piecewise constant distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::PiecewiseConstant, b> {
		static_assert(!b, "Piescwise constant Distribution for non-floating point types is impossible");
		typedef std::piecewise_constant_distribution<T> distribution_type;
	};

	//Piecewise constant distribution
	template<typename T, bool b> struct DistributionSelector<T,Distribution::PiecewiseLinear, b> {
		static_assert(!b, "Piecewise linear Distribution for non-floating point types is impossible");
		typedef std::piecewise_linear_distribution<T> distribution_type;
	};

	//
	///Generator Selector
	template<typename T, Generator::T gen, uint64_t typeSize>
	struct GeneratorSelector {};

	template<typename T, uint64_t typeSize>
	struct GeneratorSelector<T, Generator::MinimalStandard, typeSize> {
		typedef std::linear_congruential_engine<T, 48271, 0, 2147483647> generator_type;
	};

	template<typename T, uint64_t typeSize>
	struct GeneratorSelector<T, Generator::MinimalStandard0, typeSize> {
		typedef std::linear_congruential_engine<T, 16807, 0, 2147483647> generator_type;
	};

	template<typename T>
	struct GeneratorSelector<T, Generator::MersenneTwister19937, 4> {
		typedef std::mt19937 generator_type;
	};

	template<typename T>
	struct GeneratorSelector<T, Generator::MersenneTwister19937, 8> {
		typedef std::mt19937_64 generator_type;
	};
}

template<typename Type, Distribution::T distribution = Distribution::Uniform , Generator::T Gen = Generator::MersenneTwister19937>
class Random {
private:
	typedef typename detail::DistributionSelector<Type, distribution, std::is_integral<Type>::value>::distribution_type distribution_type;
	typedef typename detail::GeneratorSelector<Type, Gen, sizeof(Type)>::generator_type generator_type;
public:
	typedef typename distribution_type::param_type Parameter;
	typedef typename distribution_type::result_type result_type;
public:
	inline Random(const Parameter& param = Parameter()) : m_Distribution(param) { }

	inline result_type operator()() {
		return m_Distribution(m_Generator);
	}

private:
	generator_type m_Generator;
	distribution_type m_Distribution;
};


