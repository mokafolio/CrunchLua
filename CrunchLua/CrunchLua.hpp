#ifndef CRUNCHLUA_CRUNCHLUA_HPP
#define CRUNCHLUA_CRUNCHLUA_HPP

#include <Luanatic/Luanatic.hpp>
#include <Crunch/Circle.hpp>
#include <Crunch/Colors.hpp>
#include <Crunch/Vector2.hpp>
#include <Crunch/Vector3.hpp>
#include <Crunch/Vector4.hpp>
#include <Crunch/Quaternion.hpp>
#include <Crunch/Matrix2.hpp>
#include <Crunch/Matrix3.hpp>
#include <Crunch/Matrix4.hpp>
#include <Crunch/MatrixFunc.hpp>
#include <Crunch/Line.hpp>
#include <Crunch/LineSegment.hpp>
#include <Crunch/Rectangle.hpp>
#include <Crunch/GeometricFunc.hpp>
#include <Crunch/CommonFunc.hpp>
#include <Crunch/StringConversion.hpp>
#include <Crunch/PerlinNoise.hpp>
#include <Crunch/Randomizer.hpp>
#include <Crunch/Tween.hpp>
#include <Stick/Path.hpp>


// TODO: Add value type converter for colors
// Finish the binding for all types.

namespace crunchLua
{
    STICK_API inline void registerCrunch(lua_State * _state, const stick::String & _namespace = "");

    using Float = stick::Float32;
    using Circle = crunch::Circle<Float>;
    using Vec2 = crunch::Vector2<Float>;
    using Vec3 = crunch::Vector3<Float>;
    using Vec4 = crunch::Vector4<Float>;
    using Mat2 = crunch::Matrix2<Float>;
    using Mat3 = crunch::Matrix3<Float>;
    using Mat4 = crunch::Matrix4<Float>;
    using Quat = crunch::Quaternion<Float>;
    using ColorRGB = crunch::ColorRGBT<Float>;
    using ColorRGBA = crunch::ColorRGBAT<Float>;
    using ColorHSB = crunch::ColorHSBT<Float>;
    using ColorHSBA = crunch::ColorHSBAT<Float>;
    using Rect = crunch::Rectangle<Float>;
    using Line2 = crunch::Line<Vec2>;
    using Line3 = crunch::Line<Vec3>;
    using LineSegment2 = crunch::LineSegment<Vec2>;

    namespace detail
    {
        using namespace stick;

        template<class T>
        static Int32 luaToStringHelper(lua_State * _luaState)
        {
            T * val = luanatic::convertToTypeAndCheck<T>(_luaState, 1);
            lua_pushstring(_luaState, crunch::toString(*val).cString());
            return 1;
        }

        static crunch::Randomizer & luaRandomNumberGenerator()
        {
            static crunch::Randomizer s_luaRandomNumbers;
            return s_luaRandomNumbers;
        }

        static Int32 luaRandom(lua_State * _luaState)
        {
            // lua_getfield(_luaState, LUA_REGISTRYINDEX, LUANATIC_KEY);
            // STICK_ASSERT(lua_istable(_luaState, -1));
            // lua_getfield(_luaState, -1, "crunch");
            // if(lua_isnil(_luaState, -1))
            // {
            //     lua_pop(_state, 1);
            //     lua_newtable(_luaState);
            //     lua_pushvalue(_luaState, -1);
            //     lua_setfield(_luaState, -3, "crunch");
            // }

            crunch::Randomizer & rnd = luaRandomNumberGenerator();
            if (lua_isnil(_luaState, 1))
            {
                lua_pushnumber(_luaState, rnd.randomd());
            }
            else
            {
                Float64 num = luaL_checknumber(_luaState, 1);
                if (lua_isnumber(_luaState, 2))
                {
                    lua_pushnumber(_luaState, rnd.randomd(num, lua_tonumber(_luaState, 2)));
                }
                else
                {
                    lua_pushnumber(_luaState, rnd.randomd(num));
                }
            }

            return 1;
        }

        static Int32 luaRandomSeed(lua_State * _luaState)
        {
            luaRandomNumberGenerator().setSeed(luaL_checkinteger(_luaState, 1));
            return 0;
        }

        static Int32 luaRandomizeSeed(lua_State * _luaState)
        {
            luaRandomNumberGenerator().randomizeSeed();
            return 0;
        }

        static Int32 luaNoise(lua_State * _luaState)
        {
            static crunch::PerlinNoise s_noise;

            Float64 val1 = luaL_checknumber(_luaState, 1);
            if (lua_isnumber(_luaState, 2))
            {
                Float64 val2 = lua_tonumber(_luaState, 2);
                if (lua_isnumber(_luaState, 3))
                {
                    Float64 val3 = lua_tonumber(_luaState, 3);
                    if (lua_isnumber(_luaState, 4))
                    {
                        Float64 val4 = lua_tonumber(_luaState, 4);
                        lua_pushnumber(_luaState, s_noise.noise(val1, val2, val3, val4));
                    }
                    else
                    {
                        lua_pushnumber(_luaState, s_noise.noise(val1, val2, val3));
                    }
                }

                else
                {
                    lua_pushnumber(_luaState, s_noise.noise(val1, val2));
                }
            }
            else
            {
                lua_pushnumber(_luaState, s_noise.noise(val1));
            }

            return 1;
        }

        static int luaCircleFromPoints(lua_State * _luaState)
        {
            auto result = Circle::fromPoints(luanatic::convertToValueTypeAndCheck<Vec2>(_luaState, 1),
                                             luanatic::convertToValueTypeAndCheck<Vec2>(_luaState, 2),
                                             luanatic::convertToValueTypeAndCheck<Vec2>(_luaState, 3));
            if (result)
            {
                luanatic::push(_luaState, stick::defaultAllocator().create<Circle>(result.get()), true);
            }
            else
            {
                luanatic::pushValueType(_luaState, result.error());
            }

            return 1;
        }

        static Int32 luaLine2Intersect(lua_State * _luaState)
        {
            Line2 * line = luanatic::convertToTypeAndCheck<Line2>(_luaState, 1);
            Line2 * line2 = luanatic::convertToTypeAndCheck<Line2>(_luaState, 2);

            auto result = intersect(*line, *line2);

            if (result)
                luanatic::pushValueType<Vec2>(_luaState, *result);
            else
                lua_pushnil(_luaState);

            return 1;
        }

        static Int32 luaLineSegment2Intersect(lua_State * _luaState)
        {
            LineSegment2 * line = luanatic::convertToTypeAndCheck<LineSegment2>(_luaState, 1);
            LineSegment2 * line2 = luanatic::convertToTypeAndCheck<LineSegment2>(_luaState, 2);

            auto result = intersect(*line, *line2);

            if (result)
                luanatic::pushValueType<Vec2>(_luaState, *result);
            else
                lua_pushnil(_luaState);

            return 1;
        }
    }

    STICK_API inline void registerCrunch(lua_State * _state, const stick::String & _namespace)
    {
        using namespace luanatic;
        using namespace crunch;
        using namespace stick;

        LuaValue g = globalsTable(_state);
        LuaValue namespaceTable = g;
        if (!_namespace.isEmpty())
        {
            auto tokens = path::segments(_namespace, '.');
            for (const String & token : tokens)
            {
                LuaValue table = namespaceTable.findOrCreateTable(token);
                namespaceTable = table;
            }
        }

        namespaceTable.registerFunction("abs", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &), &crunch::abs<Float>));
        namespaceTable.registerFunction("abs1", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &), &crunch::abs<Float>));
        namespaceTable.registerFunction("floor", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &), &crunch::floor<Float>));
        namespaceTable.registerFunction("floor1", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &), &crunch::floor<Float>));
        namespaceTable.registerFunction("ceil", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &), &crunch::ceil<Float>));
        namespaceTable.registerFunction("ceil1", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &), &crunch::ceil<Float>));
        namespaceTable.registerFunction("fract", LUANATIC_FUNCTION(&crunch::fract<Float>));
        namespaceTable.registerFunction("fract1", LUANATIC_FUNCTION(&crunch::fract<Float>));
        namespaceTable.registerFunction("min", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &, const Float &), &crunch::min<Float>));
        namespaceTable.registerFunction("min1", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &, const Float &), &crunch::min<Float>));
        namespaceTable.registerFunction("max", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &, const Float &), &crunch::max<Float>));
        namespaceTable.registerFunction("max1", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Float &, const Float &), &crunch::max<Float>));
        namespaceTable.registerFunction("clamp", LUANATIC_FUNCTION(&crunch::clamp<Float>));
        namespaceTable.registerFunction("clamp1", LUANATIC_FUNCTION(&crunch::clamp<Float>));

        ClassWrapper<Vec2> vector2CW("Vec2");
        vector2CW.
        addConstructor<>().
        addConstructor<Float, Float>().
        addConstructor<const Vec2 &>().
        addConstructor<Float>().
        addConstructor("new").
        addConstructor<Float, Float>("fromNumbers").
        addConstructor<const Vec2 &>("copy").
        addConstructor<Float>("fromNumber").
        addMemberFunction(ToStringFlag, detail::luaToStringHelper<Vec2>).
        addAttribute("x", LUANATIC_ATTRIBUTE(&Vec2::x)).
        addAttribute("y", LUANATIC_ATTRIBUTE(&Vec2::y)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&Vec2::operator==)).
        addMemberFunction("addNumber", LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(Float)const, &Vec2::operator+)).
        addMemberFunction("subtractNumber", LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(Float)const, &Vec2::operator-)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(Float)const, &Vec2::operator*)).
        addMemberFunction("divideByNumber", LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(Float)const, &Vec2::operator/)).
        addMemberFunction("add", LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(const Vec2 &)const, &Vec2::operator+)).
        addMemberFunction("subtract", LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(const Vec2 &)const, &Vec2::operator-)).
        addMemberFunction("multiply", LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(const Vec2 &)const, &Vec2::operator*)).
        addMemberFunction("divide", LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(const Vec2 &)const, &Vec2::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(const Vec2 &)const, &Vec2::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(const Vec2 &)const, &Vec2::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(const Vec2 &)const, &Vec2::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(const Vec2 &)const, &Vec2::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(Float)const, &Vec2::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(Float)const, &Vec2::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(Float)const, &Vec2::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec2(Vec2::*)(Float)const, &Vec2::operator/));

        namespaceTable.registerClass(vector2CW);
        namespaceTable.registerFunction("cross", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &, const Vec2 &), &crunch::cross));
        namespaceTable.registerFunction("cross2", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &, const Vec2 &), &crunch::cross));
        namespaceTable.registerFunction("normalize", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &), &crunch::normalize));
        namespaceTable.registerFunction("normalize2", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &), &crunch::normalize));
        namespaceTable.registerFunction("rotate", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &, Float), &crunch::rotate));
        namespaceTable.registerFunction("rotate2", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &, Float), &crunch::rotate));
        namespaceTable.registerFunction("rotate", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &, const Vec2 &, Float), &crunch::rotate));
        namespaceTable.registerFunction("rotateAround2", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &, const Vec2 &, Float), &crunch::rotate));
        namespaceTable.registerFunction("dot", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &, const Vec2 &), &crunch::dot));
        namespaceTable.registerFunction("dot2", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &, const Vec2 &), &crunch::dot));
        namespaceTable.registerFunction("length", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &), &crunch::length));
        namespaceTable.registerFunction("length2", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &), &crunch::length));
        namespaceTable.registerFunction("distance", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &, const Vec2 &), &crunch::distance));
        namespaceTable.registerFunction("distance2", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &, const Vec2 &), &crunch::distance));
        namespaceTable.registerFunction("distanceSquared", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &, const Vec2 &), &crunch::distanceSquared));
        namespaceTable.registerFunction("distanceSquared2", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec2 &, const Vec2 &), &crunch::distanceSquared));
        namespaceTable.registerFunction("abs", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &), &crunch::abs<Float>));
        namespaceTable.registerFunction("abs2", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &), &crunch::abs<Float>));
        namespaceTable.registerFunction("floor", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &), &crunch::floor<Float>));
        namespaceTable.registerFunction("floor2", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &), &crunch::floor<Float>));
        namespaceTable.registerFunction("ceil", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &), &crunch::ceil<Float>));
        namespaceTable.registerFunction("ceil2", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &), &crunch::ceil<Float>));
        namespaceTable.registerFunction("fract", LUANATIC_FUNCTION(&crunch::fract<Vec2>));
        namespaceTable.registerFunction("fract2", LUANATIC_FUNCTION(&crunch::fract<Vec2>));
        namespaceTable.registerFunction("min", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &, const Vec2 &), &crunch::min<Float>));
        namespaceTable.registerFunction("min2", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &, const Vec2 &), &crunch::min<Float>));
        namespaceTable.registerFunction("max", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &, const Vec2 &), &crunch::max<Float>));
        namespaceTable.registerFunction("max2", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2 &, const Vec2 &), &crunch::max<Float>));
        namespaceTable.registerFunction("clamp", LUANATIC_FUNCTION(&crunch::clamp<Vec2>));
        namespaceTable.registerFunction("clamp2", LUANATIC_FUNCTION(&crunch::clamp<Vec2>));
        namespaceTable.registerFunction("reflect", LUANATIC_FUNCTION_OVERLOAD(Vec2(*)(const Vec2&, const Vec2&), &crunch::reflect<Float>));
        // namespaceTable.registerFunction("reflect2", LUANATIC_FUNCTION(&crunch::reflect<Float>));

        ClassWrapper<Vec3> vector3CW("Vec3");
        vector3CW.
        addConstructor<>().
        addConstructor<Float, Float, Float>().
        addConstructor<const Vec3 &>().
        addConstructor<Float>().
        addConstructor("new").
        addConstructor<Float, Float, Float>("fromNumbers").
        addConstructor<const Vec3 &>("copy").
        addConstructor<Float>("fromNumber").
        addMemberFunction(ToStringFlag, detail::luaToStringHelper<Vec3>).
        addAttribute("x", LUANATIC_ATTRIBUTE(&Vec3::x)).
        addAttribute("y", LUANATIC_ATTRIBUTE(&Vec3::y)).
        addAttribute("z", LUANATIC_ATTRIBUTE(&Vec3::z)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&Vec3::operator==)).
        addMemberFunction("addNumber", LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(Float)const, &Vec3::operator+)).
        addMemberFunction("subtractNumber", LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(Float)const, &Vec3::operator-)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(Float)const, &Vec3::operator*)).
        addMemberFunction("divideByNumber", LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(Float)const, &Vec3::operator/)).
        addMemberFunction("add", LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(const Vec3 &)const, &Vec3::operator+)).
        addMemberFunction("subtract", LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(const Vec3 &)const, &Vec3::operator-)).
        addMemberFunction("multiply", LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(const Vec3 &)const, &Vec3::operator*)).
        addMemberFunction("divide", LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(const Vec3 &)const, &Vec3::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(const Vec3 &)const, &Vec3::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(const Vec3 &)const, &Vec3::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(const Vec3 &)const, &Vec3::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(const Vec3 &)const, &Vec3::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(Float)const, &Vec3::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(Float)const, &Vec3::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(Float)const, &Vec3::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Vec3::*)(Float)const, &Vec3::operator/));

        namespaceTable.registerClass(vector3CW);
        namespaceTable.registerFunction("cross", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &, const Vec3 &), &crunch::cross));
        namespaceTable.registerFunction("cross3", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &, const Vec3 &), &crunch::cross));
        namespaceTable.registerFunction("normalize", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &), &crunch::normalize));
        namespaceTable.registerFunction("normalize3", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &), &crunch::normalize));
        namespaceTable.registerFunction("dot", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec3 &, const Vec3 &), &crunch::dot));
        namespaceTable.registerFunction("dot3", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec3 &, const Vec3 &), &crunch::dot));
        namespaceTable.registerFunction("length", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec3 &), &crunch::length));
        namespaceTable.registerFunction("length3", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec3 &), &crunch::length));
        namespaceTable.registerFunction("distance", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec3 &, const Vec3 &), &crunch::distance));
        namespaceTable.registerFunction("distance3", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec3 &, const Vec3 &), &crunch::distance));
        namespaceTable.registerFunction("distanceSquared", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec3 &, const Vec3 &), &crunch::distanceSquared));
        namespaceTable.registerFunction("distanceSquared3", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec3 &, const Vec3 &), &crunch::distanceSquared));
        namespaceTable.registerFunction("abs", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &), &crunch::abs<Float>));
        namespaceTable.registerFunction("abs3", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &), &crunch::abs<Float>));
        namespaceTable.registerFunction("floor", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &), &crunch::floor<Float>));
        namespaceTable.registerFunction("floor3", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &), &crunch::floor<Float>));
        namespaceTable.registerFunction("ceil", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &), &crunch::ceil<Float>));
        namespaceTable.registerFunction("ceil3", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &), &crunch::ceil<Float>));
        namespaceTable.registerFunction("fract", LUANATIC_FUNCTION(&crunch::fract<Vec3>));
        namespaceTable.registerFunction("fract3", LUANATIC_FUNCTION(&crunch::fract<Vec3>));
        namespaceTable.registerFunction("min", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &, const Vec3 &), &crunch::min<Float>));
        namespaceTable.registerFunction("min3", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &, const Vec3 &), &crunch::min<Float>));
        namespaceTable.registerFunction("max", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &, const Vec3 &), &crunch::max<Float>));
        namespaceTable.registerFunction("max3", LUANATIC_FUNCTION_OVERLOAD(Vec3(*)(const Vec3 &, const Vec3 &), &crunch::max<Float>));
        namespaceTable.registerFunction("clamp", LUANATIC_FUNCTION(&crunch::clamp<Vec3>));
        namespaceTable.registerFunction("clamp3", LUANATIC_FUNCTION(&crunch::clamp<Vec3>));

        ClassWrapper<Vec4> vector4CW("Vec4");
        vector4CW.
        addConstructor<>().
        addConstructor<const Vec4 &>().
        addConstructor<Float>().
        addConstructor<Float, Float, Float, Float>().
        addConstructor("new").
        addConstructor<const Vec4 &>("copy").
        addConstructor<Float>("fromNumber").
        addConstructor<Float, Float, Float, Float>("fromNumbers").
        addMemberFunction(ToStringFlag, detail::luaToStringHelper<Vec4>).
        addAttribute("x", LUANATIC_ATTRIBUTE(&Vec4::x)).
        addAttribute("y", LUANATIC_ATTRIBUTE(&Vec4::y)).
        addAttribute("z", LUANATIC_ATTRIBUTE(&Vec4::z)).
        addAttribute("w", LUANATIC_ATTRIBUTE(&Vec4::w)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&Vec4::operator==)).
        addMemberFunction("addNumber", LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator+)).
        addMemberFunction("subtractNumber", LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator-)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator*)).
        addMemberFunction("divideByNumber", LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator/)).
        addMemberFunction("add", LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator+)).
        addMemberFunction("subtract", LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator-)).
        addMemberFunction("multiply", LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator*)).
        addMemberFunction("divide", LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(const Vec4 &)const, &Vec4::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(const Vec4 &)const, &Vec4::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(const Vec4 &)const, &Vec4::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(const Vec4 &)const, &Vec4::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec4(Vec4::*)(Float)const, &Vec4::operator/));

        namespaceTable.registerClass(vector4CW);
        namespaceTable.registerFunction("normalize", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &), &crunch::normalize));
        namespaceTable.registerFunction("normalize4", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &), &crunch::normalize));
        namespaceTable.registerFunction("dot", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec4 &, const Vec4 &), &crunch::dot));
        namespaceTable.registerFunction("dot4", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec4 &, const Vec4 &), &crunch::dot));
        namespaceTable.registerFunction("length", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec4 &), &crunch::length));
        namespaceTable.registerFunction("length4", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec4 &), &crunch::length));
        namespaceTable.registerFunction("distance", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec4 &, const Vec4 &), &crunch::distance));
        namespaceTable.registerFunction("distance4", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec4 &, const Vec4 &), &crunch::distance));
        namespaceTable.registerFunction("distanceSquared", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec4 &, const Vec4 &), &crunch::distanceSquared));
        namespaceTable.registerFunction("distanceSquared4", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Vec4 &, const Vec4 &), &crunch::distanceSquared));
        namespaceTable.registerFunction("abs", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &), &crunch::abs<Float>));
        namespaceTable.registerFunction("abs4", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &), &crunch::abs<Float>));
        namespaceTable.registerFunction("floor", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &), &crunch::floor<Float>));
        namespaceTable.registerFunction("floor4", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &), &crunch::floor<Float>));
        namespaceTable.registerFunction("ceil", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &), &crunch::ceil<Float>));
        namespaceTable.registerFunction("ceil4", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &), &crunch::ceil<Float>));
        namespaceTable.registerFunction("fract", LUANATIC_FUNCTION(&crunch::fract<Vec4>));
        namespaceTable.registerFunction("fract4", LUANATIC_FUNCTION(&crunch::fract<Vec4>));
        namespaceTable.registerFunction("min", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &, const Vec4 &), &crunch::min<Float>));
        namespaceTable.registerFunction("min4", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &, const Vec4 &), &crunch::min<Float>));
        namespaceTable.registerFunction("max", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &, const Vec4 &), &crunch::max<Float>));
        namespaceTable.registerFunction("max4", LUANATIC_FUNCTION_OVERLOAD(Vec4(*)(const Vec4 &, const Vec4 &), &crunch::max<Float>));
        namespaceTable.registerFunction("clamp", LUANATIC_FUNCTION(&crunch::clamp<Vec4>));
        namespaceTable.registerFunction("clamp4", LUANATIC_FUNCTION(&crunch::clamp<Vec4>));

        ClassWrapper<Quat> quatCW("Quat");
        quatCW.
        addConstructor<>().
        addConstructor<Float, Float, Float, Float>().
        addConstructor<const Vec4 &>().
        addConstructor<const Quat &>().
        addConstructor<const Mat3 &>().
        addConstructor("new").
        addConstructor<Float, Float, Float, Float>("fromNumbers").
        addConstructor<const Vec4 &>("fromVector").
        addConstructor<const Quat &>("copy").
        addConstructor<const Mat3 &>("fromMatrix").
        addAttribute("x", LUANATIC_ATTRIBUTE(&Vec4::x)).
        addAttribute("y", LUANATIC_ATTRIBUTE(&Vec4::y)).
        addAttribute("z", LUANATIC_ATTRIBUTE(&Vec4::z)).
        addAttribute("w", LUANATIC_ATTRIBUTE(&Vec4::w)).
        addStaticFunction("identity", LUANATIC_FUNCTION(&Quat::identity)).
        addStaticFunction("fromAngleAndAxis", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(Float, const Vec3 &), Quat::rotation)).
        addStaticFunction("fromEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(const Vec3 &), Quat::rotation)).
        addStaticFunction("fromEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(Float, Float, Float), Quat::rotation)).
        addStaticFunction("fromEulerAnglesVec", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(const Vec3 &), Quat::rotation)).
        addStaticFunction("fromEulerAnglesNumbers", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(Float, Float, Float), Quat::rotation)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&Quat::operator==)).
        addMemberFunction("addNumber", LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(Float)const, &Quat::operator+)).
        addMemberFunction("addQuat", LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(const Quat &)const, &Quat::operator+)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(Float)const, &Quat::operator+)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(const Quat &)const, &Quat::operator+)).
        addMemberFunction("subtractNumber", LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(Float)const, &Quat::operator-)).
        addMemberFunction("subtractQuat", LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(const Quat &)const, &Quat::operator-)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(Float)const, &Quat::operator-)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(const Quat &)const, &Quat::operator-)).
        addMemberFunction("mulitplyByQuat", LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(const Quat &)const, &Quat::operator*)).
        addMemberFunction("mulitplyByVec", LUANATIC_FUNCTION_OVERLOAD(Vec3(Quat::*)(const Vec3 &)const, &Quat::operator*)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(Float)const, &Quat::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(const Quat &)const, &Quat::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Quat::*)(const Vec3 &)const, &Quat::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(Float)const, &Quat::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Quat(Quat::*)(Float)const, &Quat::operator/));

        namespaceTable.registerFunction("nmix", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(const Quat &, const Quat &, Float), &crunch::nmix));
        namespaceTable.registerFunction("smix", LUANATIC_FUNCTION(&crunch::smix<Float>));
        namespaceTable.registerFunction("length", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Quat &), &crunch::length));
        namespaceTable.registerFunction("lengthQ", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Quat &), &crunch::length));
        namespaceTable.registerFunction("dot", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Quat &, const Quat &), &crunch::dot));
        namespaceTable.registerFunction("dotQ", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Quat &, const Quat &), &crunch::dot));
        namespaceTable.registerFunction("normalize", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(const Quat &), &crunch::normalize));
        namespaceTable.registerFunction("normalizeQ", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(const Quat &), &crunch::normalize));
        namespaceTable.registerFunction("mix", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(const Quat &, const Quat &, Float), &crunch::mix));
        namespaceTable.registerFunction("mixQ", LUANATIC_FUNCTION_OVERLOAD(Quat(*)(const Quat &, const Quat &, Float), &crunch::mix));

        ClassWrapper<Mat2> matrix2CW("Matrix2");
        matrix2CW.
        addConstructor<>().
        addConstructor<const Vec2 &, const Vec2 &>().
        addConstructor<Float, Float, Float, Float>().
        addConstructor<Float>().
        addConstructor<const Mat2 &>().
        addConstructor("new").
        addConstructor<const Vec2 &, const Vec2 &>("fromColumns").
        addConstructor<Float, Float, Float, Float>("fromNumbers").
        addConstructor<Float>("fromNumber").
        addConstructor<const Mat2 &>("copy").
        addStaticFunction("identity", LUANATIC_FUNCTION(&Mat2::identity)).
        addStaticFunction("zero", LUANATIC_FUNCTION(&Mat2::zero)).
        addStaticFunction("rotation", LUANATIC_FUNCTION(&Mat2::rotation)).
        addStaticFunction("scalingFromNumber", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(Float), Mat2::scaling)).
        addStaticFunction("scalingFromNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(Float, Float), Mat2::scaling)).
        addStaticFunction("scalingFromVec", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(const Vec2 &), Mat2::scaling)).
        addStaticFunction("scaling", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(Float), Mat2::scaling)).
        addStaticFunction("scaling", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(Float, Float), Mat2::scaling)).
        addStaticFunction("scaling", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(const Vec2 &), Mat2::scaling)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&Mat2::operator==)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat2(Mat2::*)(const Mat2 &)const, &Mat2::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat2(Mat2::*)(const Mat2 &)const, &Mat2::operator-)).
        addMemberFunction("multiplyByMatrix", LUANATIC_FUNCTION_OVERLOAD(Mat2(Mat2::*)(const Mat2 &)const, &Mat2::operator*)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(Mat2(Mat2::*)(Float)const, &Mat2::operator*)).
        addMemberFunction("multiplyByVec", LUANATIC_FUNCTION_OVERLOAD(Vec2(Mat2::*)(const Vec2 &)const, &Mat2::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat2(Mat2::*)(const Mat2 &)const, &Mat2::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat2(Mat2::*)(Float)const, &Mat2::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec2(Mat2::*)(const Vec2 &)const, &Mat2::operator*)).
        addMemberFunction("element", LUANATIC_FUNCTION_OVERLOAD(const Float & (Mat2::*)(UInt32, UInt32) const, &Mat2::element)).
        addMemberFunction("scaleByNumber", LUANATIC_FUNCTION_OVERLOAD(Mat2 & (Mat2::*)(Float), &Mat2::scale)).
        addMemberFunction("scaleByNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat2 & (Mat2::*)(Float, Float), &Mat2::scale)).
        addMemberFunction("scaleByVec", LUANATIC_FUNCTION_OVERLOAD(Mat2 & (Mat2::*)(const Vec2 &), &Mat2::scale)).
        addMemberFunction("scale", LUANATIC_FUNCTION_OVERLOAD(Mat2 & (Mat2::*)(Float), &Mat2::scale)).
        addMemberFunction("scale", LUANATIC_FUNCTION_OVERLOAD(Mat2 & (Mat2::*)(Float, Float), &Mat2::scale)).
        addMemberFunction("scale", LUANATIC_FUNCTION_OVERLOAD(Mat2 & (Mat2::*)(const Vec2 &), &Mat2::scale)).
        addMemberFunction("rotate", LUANATIC_FUNCTION_OVERLOAD(Mat2 & (Mat2::*)(Float), &Mat2::rotate));

        namespaceTable.registerClass(matrix2CW);
        namespaceTable.registerFunction("transpose", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(const Mat2 &), &crunch::transpose));
        namespaceTable.registerFunction("transpose2", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(const Mat2 &), &crunch::transpose));
        namespaceTable.registerFunction("determinant2", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Mat2 &), &crunch::determinant));
        namespaceTable.registerFunction("determinant2", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Mat2 &), &crunch::determinant));
        namespaceTable.registerFunction("adjoint", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(const Mat2 &), &crunch::adjoint));
        namespaceTable.registerFunction("adjoint2", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(const Mat2 &), &crunch::adjoint));
        namespaceTable.registerFunction("inverse", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(const Mat2 &), &crunch::inverse));
        namespaceTable.registerFunction("inverse2", LUANATIC_FUNCTION_OVERLOAD(Mat2(*)(const Mat2 &), &crunch::inverse));

        ClassWrapper<Mat3> matrix3CW("Mat3");
        matrix3CW.
        addConstructor<>().
        addConstructor<const Vec3 &, const Vec3 &, const Vec3 &>().
        addConstructor<Float, Float, Float, Float, Float, Float, Float, Float, Float>().
        addConstructor<Float>().
        addConstructor<const Mat3 &>().
        addConstructor<const Quat &>().
        addConstructor("new").
        addConstructor<const Vec3 &, const Vec3 &, const Vec3 &>("fromColumns").
        addConstructor<Float, Float, Float, Float, Float, Float, Float, Float, Float>("fromNumbers").
        addConstructor<Float>("fromNumber").
        addConstructor<const Mat3 &>("fromMatrix").
        addConstructor<const Quat &>("fromQuat").
        addStaticFunction("identity", LUANATIC_FUNCTION(&Mat3::identity)).
        addStaticFunction("zero", LUANATIC_FUNCTION(&Mat3::zero)).
        addStaticFunction("translation2DFromVec", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(const Vec2 &), &Mat3::translation2D)).
        addStaticFunction("translation2DFromNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(Float, Float), &Mat3::translation2D)).
        addStaticFunction("translation2D", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(const Vec2 &), &Mat3::translation2D)).
        addStaticFunction("translation2D", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(Float, Float), &Mat3::translation2D)).
        addStaticFunction("fromAngleAndAxis", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(Float, const Vec3 &), &Mat3::rotation)).
        addStaticFunction("fromEulerAnglesNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(Float, Float, Float), &Mat3::rotation)).
        addStaticFunction("fromEulerAnglesVec", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(const Vec3 &), &Mat3::rotation)).
        addStaticFunction("fromEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(Float, Float, Float), &Mat3::rotation)).
        addStaticFunction("fromEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(const Vec3 &), &Mat3::rotation)).
        addStaticFunction("scalingFromNumber", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(Float), &Mat3::scaling)).
        addStaticFunction("scalingFromVec", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(const Vec3 &), &Mat3::scaling)).
        addStaticFunction("scalingFromNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(Float, Float, Float), &Mat3::scaling)).
        addStaticFunction("scaling", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(Float), &Mat3::scaling)).
        addStaticFunction("scaling", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(const Vec3 &), &Mat3::scaling)).
        addStaticFunction("scaling", LUANATIC_FUNCTION_OVERLOAD(Mat3(*)(Float, Float, Float), &Mat3::scaling)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&Mat3::operator==)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat3(Mat3::*)(const Mat3 &)const, &Mat3::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat3(Mat3::*)(const Mat3 &)const, &Mat3::operator-)).
        addMemberFunction("multiplyByMatrix", LUANATIC_FUNCTION_OVERLOAD(Mat3(Mat3::*)(const Mat3 &)const, &Mat3::operator*)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(Mat3(Mat3::*)(Float)const, &Mat3::operator*)).
        addMemberFunction("multiplyByVec", LUANATIC_FUNCTION_OVERLOAD(Vec3(Mat3::*)(const Vec3 &)const, &Mat3::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat3(Mat3::*)(const Mat3 &)const, &Mat3::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat3(Mat3::*)(Float)const, &Mat3::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Mat3::*)(const Vec3 &)const, &Mat3::operator*)).
        //addMemberFunction(ToStringFlag, detail::luaToStringHelper<Mat3>).
        addMemberFunction("element", LUANATIC_FUNCTION_OVERLOAD(const Float & (Mat3::*)(UInt32, UInt32) const, &Mat3::element)).
        addMemberFunction("translate2DByNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, Float), &Mat3::translate2D)).
        addMemberFunction("translate2DByVec", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Vec2 &), &Mat3::translate2D)).
        addMemberFunction("translate2D", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, Float), &Mat3::translate2D)).
        addMemberFunction("translate2D", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Vec2 &), &Mat3::translate2D)).
        addMemberFunction("scale2DByNumber", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float), &Mat3::scale2D)).
        addMemberFunction("scale2DByNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, Float), &Mat3::scale2D)).
        addMemberFunction("scale2DByVec", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Vec2 &), &Mat3::scale2D)).
        addMemberFunction("scale2D", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float), &Mat3::scale2D)).
        addMemberFunction("scale2D", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, Float), &Mat3::scale2D)).
        addMemberFunction("scale2D", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Vec2 &), &Mat3::scale2D)).
        addMemberFunction("scaleByNumber", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float), &Mat3::scale)).
        addMemberFunction("scaleByNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, Float, Float), &Mat3::scale)).
        addMemberFunction("scaleByVec", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Vec3 &), &Mat3::scale)).
        addMemberFunction("scale", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float), &Mat3::scale)).
        addMemberFunction("scale", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, Float, Float), &Mat3::scale)).
        addMemberFunction("scale", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Vec3 &), &Mat3::scale)).
        addMemberFunction("rotate2D", LUANATIC_FUNCTION(&Mat3::rotate2D)).
        addMemberFunction("rotateByQuaternion", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Quat &), &Mat3::rotate)).
        addMemberFunction("rotateByAngleAndAxis", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, const Vec3 &), &Mat3::rotate)).
        addMemberFunction("rotate", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Quat &), &Mat3::rotate)).
        addMemberFunction("rotate", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, const Vec3 &), &Mat3::rotate)).
        addMemberFunction("rotateByEulerAnglesNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, Float, Float), &Mat3::rotate)).
        addMemberFunction("rotateByEulerAnglesVec", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Vec3 &), &Mat3::rotate)).
        addMemberFunction("rotateByEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(Float, Float, Float), &Mat3::rotate)).
        addMemberFunction("rotateByEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Mat3 & (Mat3::*)(const Vec3 &), &Mat3::rotate));

        namespaceTable.registerClass(matrix3CW);
        namespaceTable.registerFunction("transpose", LUANATIC_FUNCTION_OVERLOAD(Mat3f(*)(const Mat3f &), &crunch::transpose));
        namespaceTable.registerFunction("transpose3", LUANATIC_FUNCTION_OVERLOAD(Mat3f(*)(const Mat3f &), &crunch::transpose));
        namespaceTable.registerFunction("determinant", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Mat3f &), &crunch::determinant));
        namespaceTable.registerFunction("determinant3", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Mat3f &), &crunch::determinant));
        namespaceTable.registerFunction("adjoint", LUANATIC_FUNCTION_OVERLOAD(Mat3f(*)(const Mat3f &), &crunch::adjoint));
        namespaceTable.registerFunction("adjoint3", LUANATIC_FUNCTION_OVERLOAD(Mat3f(*)(const Mat3f &), &crunch::adjoint));
        namespaceTable.registerFunction("inverse", LUANATIC_FUNCTION_OVERLOAD(Mat3f(*)(const Mat3f &), &crunch::inverse));
        namespaceTable.registerFunction("inverse3", LUANATIC_FUNCTION_OVERLOAD(Mat3f(*)(const Mat3f &), &crunch::inverse));

        ClassWrapper<Mat4> matrix4CW("Mat4");
        matrix4CW.
        addConstructor<>().
        addConstructor<const Vec4 &, const Vec4 &, const Vec4 &, const Vec4 &>().
        addConstructor<Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float>().
        addConstructor<const Mat4 &>().
        addConstructor<const Quat &>().
        addConstructor<Float>().
        addConstructor("new").
        addConstructor<const Vec4 &, const Vec4 &, const Vec4 &, const Vec4 &>("fromColumns").
        addConstructor<Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float>("fromNumbers").
        addConstructor<const Mat4 &>("copy").
        addConstructor<const Quat &>("fromQuat").
        addConstructor<Float>("fromNumber").
        addStaticFunction("identity", LUANATIC_FUNCTION(&Mat4::identity)).
        addStaticFunction("zero", LUANATIC_FUNCTION(&Mat4::zero)).
        addStaticFunction("translationFromVec", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Vec3 &), &Mat4::translation)).
        addStaticFunction("translationFromNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(Float, Float, Float), &Mat4::translation)).
        addStaticFunction("translation", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Vec3 &), &Mat4::translation)).
        addStaticFunction("translation", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(Float, Float, Float), &Mat4::translation)).
        addStaticFunction("scalingFromNumber", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(Float), &Mat4::scaling)).
        addStaticFunction("scalingFromNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(Float, Float, Float), &Mat4::scaling)).
        addStaticFunction("scalingFromVec", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Vec3 &), Mat4::scaling)).
        addStaticFunction("scaling", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(Float), &Mat4::scaling)).
        addStaticFunction("scaling", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(Float, Float, Float), &Mat4::scaling)).
        addStaticFunction("scaling", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Vec3 &), Mat4::scaling)).
        addStaticFunction("fromAngleAndAxis", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(Float, const Vec3 &), &Mat4::rotation)).
        addStaticFunction("fromEulerAnglesVec", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Vec3 &), &Mat4::rotation)).
        addStaticFunction("fromEulerAnglesNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(Float, Float, Float), &Mat4::rotation)).
        addStaticFunction("fromEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Vec3 &), &Mat4::rotation)).
        addStaticFunction("fromEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(Float, Float, Float), &Mat4::rotation)).
        addStaticFunction("ortho", LUANATIC_FUNCTION(&Mat4::ortho)).
        addStaticFunction("frustum", LUANATIC_FUNCTION(&Mat4::frustum)).
        addStaticFunction("perspective", LUANATIC_FUNCTION(&Mat4::perspective)).
        addStaticFunction("lookAt", LUANATIC_FUNCTION(&Mat4::lookAt)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&Mat4::operator==)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat4(Mat4::*)(const Mat4 &)const, &Mat4::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat4(Mat4::*)(const Mat4 &)const, &Mat4::operator-)).
        addMemberFunction("multiplyByMatrix", LUANATIC_FUNCTION_OVERLOAD(Mat4(Mat4::*)(const Mat4 &)const, &Mat4::operator*)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(Mat4(Mat4::*)(Float)const, &Mat4::operator*)).
        addMemberFunction("multiplyByVec4", LUANATIC_FUNCTION_OVERLOAD(Vec4(Mat4::*)(const Vec4 &)const, &Mat4::operator*)).
        addMemberFunction("multiplyByVec3", LUANATIC_FUNCTION_OVERLOAD(Vec3(Mat4::*)(const Vec3 &)const, &Mat4::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat4(Mat4::*)(const Mat4 &)const, &Mat4::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Mat4(Mat4::*)(Float)const, &Mat4::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec4(Mat4::*)(const Vec4 &)const, &Mat4::operator*)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(Vec3(Mat4::*)(const Vec3 &)const, &Mat4::operator*)).
        //addMemberFunction(ToStringFlag, detail::luaToStringHelper<Mat4>).
        addMemberFunction("element", LUANATIC_FUNCTION_OVERLOAD(const Float & (Mat4::*)(UInt32, UInt32) const, &Mat4::element)).
        addMemberFunction("translateByNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(Float, Float, Float), &Mat4::translate)).
        addMemberFunction("translateByVec3", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(const Vec3 &), &Mat4::translate)).
        addMemberFunction("translate", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(Float, Float, Float), &Mat4::translate)).
        addMemberFunction("translate", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(const Vec3 &), &Mat4::translate)).
        addMemberFunction("scaleByNumber", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(Float), &Mat4::scale)).
        addMemberFunction("scaleByNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(Float, Float, Float), &Mat4::scale)).
        addMemberFunction("scaleByVec3", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(const Vec3 &), &Mat4::scale)).
        addMemberFunction("scale", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(Float), &Mat4::scale)).
        addMemberFunction("scale", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(Float, Float, Float), &Mat4::scale)).
        addMemberFunction("scale", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(const Vec3 &), &Mat4::scale)).
        addMemberFunction("rotate", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(Float, const Vec3 &), &Mat4::rotate)).
        addMemberFunction("rotateByEulerAnglesVec3", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(const Vec3 &), &Mat4::rotate)).
        addMemberFunction("rotateByEulerAnglesNumbers", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(Float, Float, Float), &Mat4::rotate)).
        addMemberFunction("rotateByEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(const Vec3 &), &Mat4::rotate)).
        addMemberFunction("rotateByEulerAngles", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(Float, Float, Float), &Mat4::rotate)).
        addMemberFunction("rotateByQuat", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(const Quat &), &Mat4::rotate)).
        addMemberFunction("rotate", LUANATIC_FUNCTION_OVERLOAD(Mat4 & (Mat4::*)(const Quat &), &Mat4::rotate));

        namespaceTable.registerClass(matrix4CW);
        namespaceTable.registerFunction("transpose", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Mat4 &), &crunch::transpose));
        namespaceTable.registerFunction("determinant", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Mat4 &), &crunch::determinant));
        namespaceTable.registerFunction("adjoint", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Mat4 &), &crunch::adjoint));
        namespaceTable.registerFunction("inverse", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Mat4 &), &crunch::inverse));
        namespaceTable.registerFunction("transpose4", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Mat4 &), &crunch::transpose));
        namespaceTable.registerFunction("determinant4", LUANATIC_FUNCTION_OVERLOAD(Float(*)(const Mat4 &), &crunch::determinant));
        namespaceTable.registerFunction("adjoint4", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Mat4 &), &crunch::adjoint));
        namespaceTable.registerFunction("inverse4", LUANATIC_FUNCTION_OVERLOAD(Mat4(*)(const Mat4 &), &crunch::inverse));

        ClassWrapper<ColorRGB> colRGBCW("ColorRGB");
        colRGBCW.
        addCast<crunch::detail::RGB<Float> >().
        addConstructor<>().
        addConstructor<Float>().
        addConstructor<Float, Float, Float>().
        addConstructor<const ColorRGB &>().
        addConstructor("new").
        addConstructor<Float>("fromNumber").
        addConstructor<Float, Float, Float>("fromNumbers").
        addConstructor<const ColorRGB &>("copy").
        addMemberFunction(ToStringFlag, detail::luaToStringHelper<ColorRGB>).
        addStaticFunction("fromRGBA", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(*)(const ColorRGBA &), &toRGB)).
        addStaticFunction("fromHSB", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(*)(const ColorHSB &), &toRGB)).
        addStaticFunction("fromHSBA", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(*)(const ColorHSBA &), &toRGB)).
        addAttribute("r", LUANATIC_ATTRIBUTE(&ColorRGB::r)).
        addAttribute("g", LUANATIC_ATTRIBUTE(&ColorRGB::g)).
        addAttribute("b", LUANATIC_ATTRIBUTE(&ColorRGB::b)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&ColorRGB::operator==)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(const ColorRGB &)const, &ColorRGB::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(const ColorRGB &)const, &ColorRGB::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(const ColorRGB &)const, &ColorRGB::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(const ColorRGB &)const, &ColorRGB::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(Float)const, &ColorRGB::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(Float)const, &ColorRGB::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(Float)const, &ColorRGB::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(Float)const, &ColorRGB::operator/)).
        addMemberFunction("add", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(const ColorRGB &)const, &ColorRGB::operator+)).
        addMemberFunction("subtract", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(const ColorRGB &)const, &ColorRGB::operator-)).
        addMemberFunction("multiply", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(const ColorRGB &)const, &ColorRGB::operator*)).
        addMemberFunction("divide", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(const ColorRGB &)const, &ColorRGB::operator/)).
        addMemberFunction("addNumber", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(Float)const, &ColorRGB::operator+)).
        addMemberFunction("subtractNumber", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(Float)const, &ColorRGB::operator-)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(Float)const, &ColorRGB::operator*)).
        addMemberFunction("divideByNumber", LUANATIC_FUNCTION_OVERLOAD(ColorRGB(ColorRGB::*)(Float)const, &ColorRGB::operator/));

        namespaceTable.registerClass(colRGBCW);

        ClassWrapper<ColorRGBA> colRGBACW("ColorRGBA");
        colRGBACW.
        addCast<crunch::detail::RGBA<Float> >().
        addConstructor<>().
        addConstructor<Float, Float, Float, Float>().
        addConstructor<const ColorRGBA &>().
        addConstructor("new").
        addConstructor<Float, Float, Float, Float>("fromNumbers").
        addConstructor<const ColorRGBA &>("copy").
        addMemberFunction(ToStringFlag, detail::luaToStringHelper<ColorRGBA>).
        addStaticFunction("fromRGB", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(*)(const ColorRGB &), &toRGBA)).
        addStaticFunction("fromHSB", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(*)(const ColorHSB &), &toRGBA)).
        addStaticFunction("fromHSBA", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(*)(const ColorHSBA &), &toRGBA)).
        addAttribute("r", LUANATIC_ATTRIBUTE(&ColorRGBA::r)).
        addAttribute("g", LUANATIC_ATTRIBUTE(&ColorRGBA::g)).
        addAttribute("b", LUANATIC_ATTRIBUTE(&ColorRGBA::b)).
        addAttribute("a", LUANATIC_ATTRIBUTE(&ColorRGBA::a)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&ColorRGBA::operator==)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(const ColorRGBA &)const, &ColorRGBA::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(const ColorRGBA &)const, &ColorRGBA::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(const ColorRGBA &)const, &ColorRGBA::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(const ColorRGBA &)const, &ColorRGBA::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(Float)const, &ColorRGBA::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(Float)const, &ColorRGBA::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(Float)const, &ColorRGBA::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(Float)const, &ColorRGBA::operator/)).
        addMemberFunction("add", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(const ColorRGBA &)const, &ColorRGBA::operator+)).
        addMemberFunction("subtract", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(const ColorRGBA &)const, &ColorRGBA::operator-)).
        addMemberFunction("multiply", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(const ColorRGBA &)const, &ColorRGBA::operator*)).
        addMemberFunction("divide", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(const ColorRGBA &)const, &ColorRGBA::operator/)).
        addMemberFunction("addNumber", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(Float)const, &ColorRGBA::operator+)).
        addMemberFunction("subtractNumber", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(Float)const, &ColorRGBA::operator-)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(Float)const, &ColorRGBA::operator*)).
        addMemberFunction("divideByNumber", LUANATIC_FUNCTION_OVERLOAD(ColorRGBA(ColorRGBA::*)(Float)const, &ColorRGBA::operator/));

        namespaceTable.registerClass(colRGBACW);

        ClassWrapper<ColorHSB> colHSBCW("ColorHSB");
        colHSBCW.
        addCast<crunch::detail::HSB<Float> >().
        addConstructor<>().
        addConstructor<Float>().
        addConstructor<Float, Float, Float>().
        addConstructor<const ColorHSB &>().
        addConstructor("new").
        addConstructor<Float>("fromNumber").
        addConstructor<Float, Float, Float>("fromNumbers").
        addConstructor<const ColorHSB &>("copy").
        addMemberFunction(ToStringFlag, detail::luaToStringHelper<ColorHSB>).
        addStaticFunction("fromRGB", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(*)(const ColorRGB &), &toHSB)).
        addStaticFunction("fromRGBA", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(*)(const ColorRGBA &), &toHSB)).
        addStaticFunction("fromHSBA", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(*)(const ColorHSBA &), &toHSB)).
        addAttribute("h", LUANATIC_ATTRIBUTE(&ColorHSB::h)).
        addAttribute("s", LUANATIC_ATTRIBUTE(&ColorHSB::s)).
        addAttribute("b", LUANATIC_ATTRIBUTE(&ColorHSB::b)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&ColorHSB::operator==)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(const ColorHSB &)const, &ColorHSB::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(const ColorHSB &)const, &ColorHSB::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(const ColorHSB &)const, &ColorHSB::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(const ColorHSB &)const, &ColorHSB::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(Float)const, &ColorHSB::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(Float)const, &ColorHSB::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(Float)const, &ColorHSB::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(Float)const, &ColorHSB::operator/)).
        addMemberFunction("add", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(const ColorHSB &)const, &ColorHSB::operator+)).
        addMemberFunction("subtract", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(const ColorHSB &)const, &ColorHSB::operator-)).
        addMemberFunction("multiply", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(const ColorHSB &)const, &ColorHSB::operator*)).
        addMemberFunction("divide", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(const ColorHSB &)const, &ColorHSB::operator/)).
        addMemberFunction("addNumber", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(Float)const, &ColorHSB::operator+)).
        addMemberFunction("subtractNumber", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(Float)const, &ColorHSB::operator-)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(Float)const, &ColorHSB::operator*)).
        addMemberFunction("divideByNumber", LUANATIC_FUNCTION_OVERLOAD(ColorHSB(ColorHSB::*)(Float)const, &ColorHSB::operator/));

        namespaceTable.registerClass(colHSBCW);

        ClassWrapper<ColorHSBA> colHSBACW("ColorHSBA");
        colHSBACW.
        addCast<crunch::detail::HSBA<Float> >().
        addConstructor<>().
        addConstructor<Float, Float, Float, Float>().
        addConstructor<const ColorHSBA &>().
        addConstructor("new").
        addConstructor<Float, Float, Float, Float>("fromNumbers").
        addConstructor<const ColorHSBA &>("copy").
        addMemberFunction(ToStringFlag, detail::luaToStringHelper<ColorHSBA>).
        addStaticFunction("fromRGB", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(*)(const ColorRGB &), &toHSBA)).
        addStaticFunction("fromRGBA", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(*)(const ColorRGBA &), &toHSBA)).
        addStaticFunction("fromHSB", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(*)(const ColorHSB &), &toHSBA)).
        addAttribute("h", LUANATIC_ATTRIBUTE(&ColorHSBA::h)).
        addAttribute("s", LUANATIC_ATTRIBUTE(&ColorHSBA::s)).
        addAttribute("b", LUANATIC_ATTRIBUTE(&ColorHSBA::b)).
        addAttribute("a", LUANATIC_ATTRIBUTE(&ColorHSBA::a)).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&ColorHSBA::operator==)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(const ColorHSBA &)const, &ColorHSBA::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(const ColorHSBA &)const, &ColorHSBA::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(const ColorHSBA &)const, &ColorHSBA::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(const ColorHSBA &)const, &ColorHSBA::operator/)).
        addMemberFunction(AdditionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(Float)const, &ColorHSBA::operator+)).
        addMemberFunction(SubtractionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(Float)const, &ColorHSBA::operator-)).
        addMemberFunction(MultiplicationOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(Float)const, &ColorHSBA::operator*)).
        addMemberFunction(DivisionOperatorFlag, LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(Float)const, &ColorHSBA::operator/)).
        addMemberFunction("add", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(const ColorHSBA &)const, &ColorHSBA::operator+)).
        addMemberFunction("subtract", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(const ColorHSBA &)const, &ColorHSBA::operator-)).
        addMemberFunction("multiply", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(const ColorHSBA &)const, &ColorHSBA::operator*)).
        addMemberFunction("divide", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(const ColorHSBA &)const, &ColorHSBA::operator/)).
        addMemberFunction("addNumber", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(Float)const, &ColorHSBA::operator+)).
        addMemberFunction("subtractNumber", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(Float)const, &ColorHSBA::operator-)).
        addMemberFunction("multiplyByNumber", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(Float)const, &ColorHSBA::operator*)).
        addMemberFunction("divideByNumber", LUANATIC_FUNCTION_OVERLOAD(ColorHSBA(ColorHSBA::*)(Float)const, &ColorHSBA::operator/));

        namespaceTable.registerClass(colHSBACW);

        ClassWrapper<Rect> rectangleCW("Rect");
        rectangleCW.
        addConstructor<>().
        addConstructor<Float, Float, Float, Float>().
        addConstructor<const Vec2 &, const Vec2 &>().
        addConstructor<const Vec2 &, Float, Float>().
        addConstructor<const Rect &>().
        addConstructor("new").
        addConstructor<Float, Float, Float, Float>("fromMinMaxNumbers").
        addConstructor<const Vec2 &, const Vec2 &>("fromMinMax").
        addConstructor<const Vec2 &, Float, Float>("fromPositionAndSize").
        addConstructor<const Rect &>("copy").
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&Rect::operator==)).
        addMemberFunction("containsRect", LUANATIC_FUNCTION_OVERLOAD(bool(Rect::*)(const Rect &)const, &Rect::contains)).
        addMemberFunction("containsPoint", LUANATIC_FUNCTION_OVERLOAD(bool(Rect::*)(const Vec2 &)const, &Rect::contains)).
        addMemberFunction("contains", LUANATIC_FUNCTION_OVERLOAD(bool(Rect::*)(const Rect &)const, &Rect::contains)).
        addMemberFunction("contains", LUANATIC_FUNCTION_OVERLOAD(bool(Rect::*)(const Vec2 &)const, &Rect::contains)).
        addMemberFunction("setSizeFromNumbers", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::setSize)).
        addMemberFunction("setSizeFromVec2", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::setSize)).
        addMemberFunction("setSize", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::setSize)).
        addMemberFunction("setSize", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::setSize)).
        addMemberFunction("setPositionFromNumbers", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::setPosition)).
        addMemberFunction("setPositionFromVec2", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::setPosition)).
        addMemberFunction("setPosition", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::setPosition)).
        addMemberFunction("setPosition", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::setPosition)).
        addMemberFunction("moveByNumbers", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::moveBy)).
        addMemberFunction("moveByVec2", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::moveBy)).
        addMemberFunction("moveBy", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::moveBy)).
        addMemberFunction("moveBy", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::moveBy)).
        addMemberFunction("setMinFromNumbers", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::setMin)).
        addMemberFunction("setMinFromVec2", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::setMin)).
        addMemberFunction("setMin", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::setMin)).
        addMemberFunction("setMin", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::setMin)).
        addMemberFunction("setMaxFromNumbers", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::setMax)).
        addMemberFunction("setMaxFromVec2", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::setMax)).
        addMemberFunction("setMax", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(Float, Float), &Rect::setMax)).
        addMemberFunction("setMax", LUANATIC_FUNCTION_OVERLOAD(void(Rect::*)(const Vec2 &), &Rect::setMax)).
        addMemberFunction("area", LUANATIC_FUNCTION(&Rect::area)).
        addMemberFunction("min", LUANATIC_FUNCTION_OVERLOAD(const Vec2 & (Rect::*)(void)const, &Rect::min)).
        addMemberFunction("max", LUANATIC_FUNCTION_OVERLOAD(const Vec2 & (Rect::*)(void)const, &Rect::max)).
        addMemberFunction("center", LUANATIC_FUNCTION(&Rect::center)).
        addMemberFunction("width", LUANATIC_FUNCTION(&Rect::width)).
        addMemberFunction("height", LUANATIC_FUNCTION(&Rect::height)).
        addMemberFunction("size", LUANATIC_FUNCTION(&Rect::size));

        namespaceTable.registerClass(rectangleCW);

        ClassWrapper<Line2> line2CW("Line2");
        line2CW.
        addConstructor<>().
        addConstructor<const Vec2 &, const Vec2 &>().
        addConstructor<const Line2 &>().
        addConstructor("new").
        addConstructor<const Vec2 &, const Vec2 &>("fromPointAndDirection").
        addConstructor<const Line2 &>("copy").
        addStaticFunction("fromPoints", LUANATIC_FUNCTION(&Line2::fromPoints)).
        addMemberFunction("position", LUANATIC_FUNCTION(&Line2::position)).
        addMemberFunction("direction", LUANATIC_FUNCTION(&Line2::direction));

        namespaceTable.registerClass(line2CW);
        namespaceTable.registerFunction("intersectLines2", &detail::luaLine2Intersect);

        ClassWrapper<Line3> line3CW("Line3");
        line3CW.
        addConstructor<>().
        addConstructor<const Vec3 &, const Vec3 &>().
        addConstructor<const Line3 &>().
        addConstructor("new").
        addConstructor<const Vec3 &, const Vec3 &>("fromPointAndDirection").
        addConstructor<const Line3 &>("copy").
        addStaticFunction("fromPoints", LUANATIC_FUNCTION(&Line3::fromPoints)).
        addMemberFunction("position", LUANATIC_FUNCTION(&Line3::position)).
        addMemberFunction("direction", LUANATIC_FUNCTION(&Line3::direction));

        namespaceTable.registerClass(line3CW);

        ClassWrapper<LineSegment2> lineSegment2CW("LineSegment2");
        lineSegment2CW.
        addConstructor<>().
        addConstructor<const Vec2 &, const Vec2 &>().
        addConstructor<const LineSegment2 &>().
        addConstructor("new").
        addConstructor<const Vec2 &, const Vec2 &>("fromPoints").
        addConstructor<const LineSegment2 &>("copy").
        addMemberFunction("positionOne", LUANATIC_FUNCTION(&LineSegment2::positionOne)).
        addMemberFunction("direction", LUANATIC_FUNCTION(&LineSegment2::direction)).
        addMemberFunction("positionTwo", LUANATIC_FUNCTION(&LineSegment2::positionTwo));

        namespaceTable.registerClass(lineSegment2CW);
        namespaceTable.registerFunction("intersectLineSegments2", &detail::luaLineSegment2Intersect);


        namespaceTable.registerFunction("random", detail::luaRandom);
        namespaceTable.registerFunction("randomSeed", detail::luaRandomSeed);
        namespaceTable.registerFunction("randomizeSeed", detail::luaRandomizeSeed);
        namespaceTable.registerFunction("noise", detail::luaNoise);
        namespaceTable.registerFunction("toRadians", LUANATIC_FUNCTION(&crunch::toRadians<Float>));
        namespaceTable.registerFunction("toDegrees", LUANATIC_FUNCTION(&crunch::toDegrees<Float>));


        ClassWrapper<Circle> circleCW("Circle");

        circleCW.
        addConstructor<>().
        addConstructor<const Vec2 &, Float32>().
        addConstructor<const Circle &>().
        addConstructor("new").
        addConstructor<const Vec2 &, Float32>("fromPositionAndRadius").
        addConstructor<const Circle &>("copy").
        addStaticFunction("fromPoints", detail::luaCircleFromPoints).
        addMemberFunction(EqualOperatorFlag, LUANATIC_FUNCTION(&Circle::operator==)).
        addMemberFunction("position", LUANATIC_FUNCTION_OVERLOAD(const Vec2 & (Circle::*)() const, &Circle::position)).
        addMemberFunction("radius", LUANATIC_FUNCTION(&Circle::radius)).
        addMemberFunction("setPosition", LUANATIC_FUNCTION(&Circle::setPosition)).
        addMemberFunction("setRadius", LUANATIC_FUNCTION(&Circle::setRadius));

        namespaceTable.registerClass(circleCW);


        // Register Tweens
        ClassWrapper<TweenLinearEaseOutf> tweenLinearEaseOutfCW("TweenLinearEaseOut");

        tweenLinearEaseOutfCW.
        addConstructor<>().
        addConstructor<const Float32 &, const Float32 &, Float64>().
        addConstructor("new").
        addConstructor<const Float32 &, const Float32 &, Float64>("fromValues").
        addMemberFunction("update", LUANATIC_FUNCTION(&TweenLinearEaseOutf::update)).
        addMemberFunction("origin", LUANATIC_FUNCTION(&TweenLinearEaseOutf::origin)).
        addMemberFunction("destination", LUANATIC_FUNCTION(&TweenLinearEaseOutf::destination)).
        addMemberFunction("current", LUANATIC_FUNCTION(&TweenLinearEaseOutf::current)).
        addMemberFunction("timeElapsed", LUANATIC_FUNCTION(&TweenLinearEaseOutf::timeElapsed)).
        addMemberFunction("duration", LUANATIC_FUNCTION(&TweenLinearEaseOutf::duration)).
        addMemberFunction("isFinished", LUANATIC_FUNCTION(&TweenLinearEaseOutf::isFinished));

        namespaceTable.registerClass(tweenLinearEaseOutfCW);


        ClassWrapper<TweenLinearEaseOut2f> tweenLinearEaseOut2fCW("TweenLinearEaseOut2");

        tweenLinearEaseOut2fCW.
        addConstructor<>().
        addConstructor<const Vec2f &, const Vec2f &, Float64>().
        addConstructor("new").
        addConstructor<const Vec2f &, const Vec2f &, Float64>("fromValues").
        addMemberFunction("update", LUANATIC_FUNCTION(&TweenLinearEaseOut2f::update)).
        addMemberFunction("origin", LUANATIC_FUNCTION(&TweenLinearEaseOut2f::origin)).
        addMemberFunction("destination", LUANATIC_FUNCTION(&TweenLinearEaseOut2f::destination)).
        addMemberFunction("current", LUANATIC_FUNCTION(&TweenLinearEaseOut2f::current)).
        addMemberFunction("timeElapsed", LUANATIC_FUNCTION(&TweenLinearEaseOut2f::timeElapsed)).
        addMemberFunction("duration", LUANATIC_FUNCTION(&TweenLinearEaseOut2f::duration)).
        addMemberFunction("isFinished", LUANATIC_FUNCTION(&TweenLinearEaseOut2f::isFinished));

        namespaceTable.registerClass(tweenLinearEaseOut2fCW);


        ClassWrapper<TweenCubicEaseOutf> tweenCubicEaseOutfCW("TweenCubicEaseOut");

        tweenCubicEaseOutfCW.
        addConstructor<>().
        addConstructor<const Float32 &, const Float32 &, Float64>().
        addConstructor("new").
        addConstructor<const Float32 &, const Float32 &, Float64>("fromValues").
        addMemberFunction("update", LUANATIC_FUNCTION(&TweenCubicEaseOutf::update)).
        addMemberFunction("origin", LUANATIC_FUNCTION(&TweenCubicEaseOutf::origin)).
        addMemberFunction("destination", LUANATIC_FUNCTION(&TweenCubicEaseOutf::destination)).
        addMemberFunction("current", LUANATIC_FUNCTION(&TweenCubicEaseOutf::current)).
        addMemberFunction("timeElapsed", LUANATIC_FUNCTION(&TweenCubicEaseOutf::timeElapsed)).
        addMemberFunction("duration", LUANATIC_FUNCTION(&TweenCubicEaseOutf::duration)).
        addMemberFunction("isFinished", LUANATIC_FUNCTION(&TweenCubicEaseOutf::isFinished));

        namespaceTable.registerClass(tweenCubicEaseOutfCW);


        ClassWrapper<TweenCubicEaseInf> tweenCubicEaseInCW("TweenCubicEaseIn");

        tweenCubicEaseInCW.
        addConstructor<>().
        addConstructor<const Float32 &, const Float32 &, Float64>().
        addConstructor("new").
        addConstructor<const Float32 &, const Float32 &, Float64>("fromValues").
        addMemberFunction("update", LUANATIC_FUNCTION(&TweenCubicEaseInf::update)).
        addMemberFunction("origin", LUANATIC_FUNCTION(&TweenCubicEaseInf::origin)).
        addMemberFunction("destination", LUANATIC_FUNCTION(&TweenCubicEaseInf::destination)).
        addMemberFunction("current", LUANATIC_FUNCTION(&TweenCubicEaseInf::current)).
        addMemberFunction("timeElapsed", LUANATIC_FUNCTION(&TweenCubicEaseInf::timeElapsed)).
        addMemberFunction("duration", LUANATIC_FUNCTION(&TweenCubicEaseInf::duration)).
        addMemberFunction("isFinished", LUANATIC_FUNCTION(&TweenCubicEaseInf::isFinished));

        namespaceTable.registerClass(tweenCubicEaseInCW);


        ClassWrapper<TweenCubicEaseInOutf> tweenCubicEaseInOutfCW("TweenCubicEaseInOut");

        tweenCubicEaseInOutfCW.
        addConstructor<>().
        addConstructor<const Float32 &, const Float32 &, Float64>().
        addConstructor("new").
        addConstructor<const Float32 &, const Float32 &, Float64>("fromValues").
        addMemberFunction("update", LUANATIC_FUNCTION(&TweenCubicEaseInOutf::update)).
        addMemberFunction("origin", LUANATIC_FUNCTION(&TweenCubicEaseInOutf::origin)).
        addMemberFunction("destination", LUANATIC_FUNCTION(&TweenCubicEaseInOutf::destination)).
        addMemberFunction("current", LUANATIC_FUNCTION(&TweenCubicEaseInOutf::current)).
        addMemberFunction("timeElapsed", LUANATIC_FUNCTION(&TweenCubicEaseInOutf::timeElapsed)).
        addMemberFunction("duration", LUANATIC_FUNCTION(&TweenCubicEaseInOutf::duration)).
        addMemberFunction("isFinished", LUANATIC_FUNCTION(&TweenCubicEaseInOutf::isFinished));

        namespaceTable.registerClass(tweenCubicEaseInOutfCW);


        ClassWrapper<TweenCubicEaseOut2f> tweenCubicEaseOut2fCW("TweenCubicEaseOut2");

        tweenCubicEaseOut2fCW.
        addConstructor<>().
        addConstructor<const Vec2f &, const Vec2f &, Float64>().
        addConstructor("new").
        addConstructor<const Vec2f &, const Vec2f &, Float64>("fromValues").
        addMemberFunction("update", LUANATIC_FUNCTION(&TweenCubicEaseOut2f::update)).
        addMemberFunction("origin", LUANATIC_FUNCTION(&TweenCubicEaseOut2f::origin)).
        addMemberFunction("destination", LUANATIC_FUNCTION(&TweenCubicEaseOut2f::destination)).
        addMemberFunction("current", LUANATIC_FUNCTION(&TweenCubicEaseOut2f::current)).
        addMemberFunction("timeElapsed", LUANATIC_FUNCTION(&TweenCubicEaseOut2f::timeElapsed)).
        addMemberFunction("duration", LUANATIC_FUNCTION(&TweenCubicEaseOut2f::duration)).
        addMemberFunction("isFinished", LUANATIC_FUNCTION(&TweenCubicEaseOut2f::isFinished));

        namespaceTable.registerClass(tweenCubicEaseOut2fCW);


        ClassWrapper<TweenElasticEaseOutf> tweenElastiEaseOutfCW("TweenElasticEaseOut");

        tweenElastiEaseOutfCW.
        addConstructor<>().
        addConstructor<const Float32 &, const Float32 &, Float64>().
        addConstructor("new").
        addConstructor<const Float32 &, const Float32 &, Float64>("fromValues").
        addMemberFunction("update", LUANATIC_FUNCTION(&TweenElasticEaseOutf::update)).
        addMemberFunction("origin", LUANATIC_FUNCTION(&TweenElasticEaseOutf::origin)).
        addMemberFunction("destination", LUANATIC_FUNCTION(&TweenElasticEaseOutf::destination)).
        addMemberFunction("current", LUANATIC_FUNCTION(&TweenElasticEaseOutf::current)).
        addMemberFunction("timeElapsed", LUANATIC_FUNCTION(&TweenElasticEaseOutf::timeElapsed)).
        addMemberFunction("duration", LUANATIC_FUNCTION(&TweenElasticEaseOutf::duration)).
        addMemberFunction("isFinished", LUANATIC_FUNCTION(&TweenElasticEaseOutf::isFinished));

        namespaceTable.registerClass(tweenElastiEaseOutfCW);


        ClassWrapper<TweenElasticEaseOut2f> tweenElastiEaseOut2fCW("TweenElasticEaseOut2");

        tweenElastiEaseOut2fCW.
        addConstructor<>().
        addConstructor<const Vec2f &, const Vec2f &, Float64>().
        addConstructor("new").
        addConstructor<const Vec2f &, const Vec2f &, Float64>("fromValues").
        addMemberFunction("update", LUANATIC_FUNCTION(&TweenElasticEaseOut2f::update)).
        addMemberFunction("origin", LUANATIC_FUNCTION(&TweenElasticEaseOut2f::origin)).
        addMemberFunction("destination", LUANATIC_FUNCTION(&TweenElasticEaseOut2f::destination)).
        addMemberFunction("current", LUANATIC_FUNCTION(&TweenElasticEaseOut2f::current)).
        addMemberFunction("timeElapsed", LUANATIC_FUNCTION(&TweenElasticEaseOut2f::timeElapsed)).
        addMemberFunction("duration", LUANATIC_FUNCTION(&TweenElasticEaseOut2f::duration)).
        addMemberFunction("isFinished", LUANATIC_FUNCTION(&TweenElasticEaseOut2f::isFinished));

        namespaceTable.registerClass(tweenElastiEaseOut2fCW);
    }
}

namespace luanatic
{
    //Value Converter implementation to seemlessly convert between lua values and a 2D vector
    template<>
    struct ValueTypeConverter<crunchLua::Vec2>
    {
        static crunchLua::Vec2 convertAndCheck(lua_State * _state, stick::Int32 _index)
        {
            crunchLua::Float x, y;
            x = y = 0;

            if (lua_gettop(_state) >= _index)
            {
                crunchLua::Vec2 * other = convertToType<crunchLua::Vec2>(_state, _index);
                if (other)
                {
                    return crunchLua::Vec2(*other);
                }
                else if (lua_isnumber(_state, _index))
                {
                    x = lua_tonumber(_state, _index);
                    y = lua_isnumber(_state, _index + 1) ? lua_tonumber(_state, _index + 1) : x;
                }
                else if (lua_istable(_state, _index))
                {
                    lua_getfield(_state, _index, "x"); //... x
                    if (!lua_isnil(_state, -1))
                    {
                        lua_getfield(_state, _index, "y"); //... y
                    }
                    else
                    {
                        lua_rawgeti(_state, _index, 1); //... nil 1
                        lua_rawgeti(_state, _index, 2); //... nil 1 2
                    }
                    x = luaL_checknumber(_state, -2);
                    y = luaL_checknumber(_state, -1);
                }
                else
                {
                    const char * msg = lua_pushfstring(_state, "Table, Vector or Numbers expected, got %s", luaL_typename(_state, _index));
                    luaL_argerror(_state, _index, msg);
                }
            }

            return crunchLua::Vec2(x, y);
        }

        static stick::Int32 push(lua_State * _state, const crunchLua::Vec2 & _vector)
        {
            printf("PUSHING VEC2 FROM VTC!!\n");
            luanatic::push<crunchLua::Vec2>(_state, stick::defaultAllocator().create<crunchLua::Vec2>(_vector));
            return 1;
        }
    };

    template<>
    struct ValueTypeConverter<crunchLua::Vec3>
    {
        static crunchLua::Vec3 convertAndCheck(lua_State * _state, stick::Int32 _index)
        {
            crunchLua::Float x, y, z;
            x = y = z = 0;

            if (lua_gettop(_state) >= _index)
            {
                crunchLua::Vec3 * other = convertToType<crunchLua::Vec3>(_state, _index);
                if (other)
                {
                    return crunchLua::Vec3(*other);
                }
                else if (lua_isnumber(_state, _index))
                {
                    x = lua_tonumber(_state, _index);
                    y = lua_isnumber(_state, _index + 1) ? lua_tonumber(_state, _index + 1) : x;
                    z = lua_isnumber(_state, _index + 2) ? lua_tonumber(_state, _index + 2) : x;
                }
                else if (lua_istable(_state, _index))
                {
                    lua_getfield(_state, _index, "x"); //... x
                    if (!lua_isnil(_state, -1))
                    {
                        lua_getfield(_state, _index, "y"); //... y
                        lua_getfield(_state, _index, "z"); //... z
                    }
                    else
                    {
                        lua_rawgeti(_state, _index, 1); //... nil 1
                        lua_rawgeti(_state, _index, 2); //... nil 1 2
                        lua_rawgeti(_state, _index, 3); //... nil 1 2 3
                    }
                    x = luaL_checknumber(_state, -3);
                    y = luaL_checknumber(_state, -2);
                    z = luaL_checknumber(_state, -1);
                }
                else
                {
                    const char * msg = lua_pushfstring(_state, "Table, Vector or Numbers expected, got %s", luaL_typename(_state, _index));
                    luaL_argerror(_state, _index, msg);
                }
            }

            return crunchLua::Vec3(x, y, z);
        }

        static stick::Int32 push(lua_State * _state, const crunchLua::Vec3 & _vector)
        {
            luanatic::push<crunchLua::Vec3>(_state, stick::defaultAllocator().create<crunchLua::Vec3>(_vector));
            return 1;
        }
    };

    template<>
    struct ValueTypeConverter<crunchLua::Vec4>
    {
        static crunchLua::Vec4 convertAndCheck(lua_State * _state, stick::Int32 _index)
        {
            crunchLua::Float x, y, z, w;
            x = y = z = w = 0;

            //is this check needed?!
            if (lua_gettop(_state) >= _index)
            {
                crunchLua::Vec4 * other = convertToType<crunchLua::Vec4>(_state, _index);
                if (other)
                {
                    return crunchLua::Vec4(*other);
                }
                else if (lua_isnumber(_state, _index))
                {
                    x = lua_tonumber(_state, _index);
                    y = lua_isnumber(_state, _index + 1) ? lua_tonumber(_state, _index + 1) : x;
                    z = lua_isnumber(_state, _index + 2) ? lua_tonumber(_state, _index + 2) : x;
                    w = lua_isnumber(_state, _index + 3) ? lua_tonumber(_state, _index + 3) : x;
                }
                else if (lua_istable(_state, _index))
                {
                    lua_getfield(_state, _index, "x"); //... x
                    if (!lua_isnil(_state, -1))
                    {
                        lua_getfield(_state, _index, "y"); //... y
                        lua_getfield(_state, _index, "z"); //... z
                        lua_getfield(_state, _index, "w"); //... w
                    }
                    else
                    {
                        lua_rawgeti(_state, _index, 1); //... nil 1
                        lua_rawgeti(_state, _index, 2); //... nil 1 2
                        lua_rawgeti(_state, _index, 3); //... nil 1 2 3
                        lua_rawgeti(_state, _index, 4); //... nil 1 2 3 4
                    }
                    x = luaL_checknumber(_state, -4);
                    y = luaL_checknumber(_state, -3);
                    z = luaL_checknumber(_state, -2);
                    w = luaL_checknumber(_state, -1);
                }
                else
                {
                    const char * msg = lua_pushfstring(_state, "Table, Vector or Numbers expected, got %s", luaL_typename(_state, _index));
                    luaL_argerror(_state, _index, msg);
                }
            }

            return crunchLua::Vec4(x, y, z, w);
        }

        static stick::Int32 push(lua_State * _state, const crunchLua::Vec4 & _vector)
        {
            luanatic::push<crunchLua::Vec4>(_state, stick::defaultAllocator().create<crunchLua::Vec4>(_vector));
            return 1;
        }
    };
}

#endif //CRUNCHLUA_CRUNCHLUA_HPP
