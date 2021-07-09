
#pragma once

#include <string>

/**
 * @file: 一维曲线基类
  */
class Curve1d
{
    public:
        Curve1d() = default;
        virtual ~Curve1d() = default;

        virtual double Evaluate(const std::uint32_t order,const double param) const = 0;

        virtual double ParamLength() const = 0;

        virtual std::string ToString() const = 0;
};