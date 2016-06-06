#include <Stick/Test.hpp>
#include <CrunchLua/CrunchLua.hpp>

using namespace stick;
using namespace crunchLua;
using namespace luanatic;

const Suite spec[] =
{
    SUITE("Namespacing Tests")
    {
        lua_State * state = createLuaState();
        {
            openStandardLibraries(state);
            initialize(state);
            registerCrunch(state, "crunch");

            //very basic test to see if the namespacing works
            String vec2Tests =
            "local v = crunch.Vec2.fromNumbers(2.0, 3.0)\n"
            "assert(v.x == 2.0)\n"
            "assert(v.y == 3.0)\n";

            auto err = luanatic::execute(state, vec2Tests);
            if (err)
                printf("%s\n", err.message().cString());

            EXPECT(!err);
        }
        lua_close(state);
    },
    SUITE("Vec2 Tests")
    {
        lua_State * state = createLuaState();
        {
            openStandardLibraries(state);
            initialize(state);
            registerCrunch(state);

            String vec2Tests =
                "local epsilon = 0.000001\n"
                "local v = Vec2.fromNumbers(1.0, 2.5)\n"
                "local v2 = Vec2.copy(v)\n"
                "print(v.x, v.y, v2.x, v2.y)\n"
                "assert(v.x == v2.x)\n"
                "assert(v.y == v2.y)\n"
                "assert(v == v2)\n"
                "local v3 = v + v2\n"
                "assert(math.abs(v3.x - 2.0) < epsilon)\n"
                "assert(math.abs(v3.y - 5.0) < epsilon)\n"
                "local v4 = v3 - Vec2.fromNumbers(0.35, 0.75)\n"
                "assert(math.abs(v4.x - 1.65) < epsilon)\n"
                "assert(math.abs(v4.y - 4.25) < epsilon)\n"
                "local v5 = v4:multiplyByNumber(2.0)\n"
                "assert(math.abs(v5.x - 3.3) < epsilon)\n"
                "assert(math.abs(v5.y - 8.5) < epsilon)\n"
                "local v6 = v5 * {2.0, 0.5}\n"
                "assert(math.abs(v6.x - 6.6) < epsilon)\n"
                "assert(math.abs(v6.y - 4.25) < epsilon)\n"
                "local v7 = v6 / {3.0, 2.0}\n"
                "assert(math.abs(v7.x - 2.2) < epsilon)\n"
                "assert(math.abs(v7.y - 2.125) < epsilon)\n"
                "local v8 = v7:multiplyByNumber(0.5)\n"
                "assert(math.abs(v8.x - 1.1) < epsilon)\n"
                "assert(math.abs(v8.y - 1.0625) < epsilon)\n"
                "local v9 = v8:addNumber(3.0)\n"
                "assert(math.abs(v9.x - 4.1) < epsilon)\n"
                "assert(math.abs(v9.y - 4.0625) < epsilon)\n"
                "local v10 = v9:subtractNumber(2.0)\n"
                "assert(math.abs(v10.x - 2.1) < epsilon)\n"
                "assert(math.abs(v10.y - 2.0625) < epsilon)\n";

            auto err = luanatic::execute(state, vec2Tests);
            if (err)
                printf("%s\n", err.message().cString());

            EXPECT(!err);
        }
        lua_close(state);
    },
    SUITE("Vec3 Tests")
    {
        lua_State * state = createLuaState();
        {
            openStandardLibraries(state);
            initialize(state);
            registerCrunch(state);

            String vec3Tests =
                "local epsilon = 0.000001\n"
                "local v = Vec3.fromNumbers(1.0, 2.5, 3.6)\n"
                "assert(v.x == 1.0)\n"
                "assert(v.y == 2.5)\n"
                "assert(math.abs(v.z - 3.6) < epsilon)\n"
                "local v2 = Vec3.copy(v)\n"
                "assert(v.x == v2.x)\n"
                "assert(v.y == v2.y)\n"
                "assert(v.z == v2.z)\n"
                "assert(v == v2)\n"
                "local v3 = v + v2\n"
                "assert(math.abs(v3.x - 2.0) < epsilon)\n"
                "assert(math.abs(v3.y - 5.0) < epsilon)\n"
                "assert(math.abs(v3.z - 7.2) < epsilon)\n"
                "local v4 = v3 - Vec3.fromNumbers(0.35, 0.75, 0.2)\n"
                "assert(math.abs(v4.x - 1.65) < epsilon)\n"
                "assert(math.abs(v4.y - 4.25) < epsilon)\n"
                "assert(math.abs(v4.z - 7.0) < epsilon)\n"
                "local v5 = v4:multiplyByNumber(2.0)\n"
                "assert(math.abs(v5.x - 3.3) < epsilon)\n"
                "assert(math.abs(v5.y - 8.5) < epsilon)\n"
                "assert(math.abs(v5.z - 14.0) < epsilon)\n"
                "local v6 = v5 * {2.0, 0.5, 2.0}\n"
                "assert(math.abs(v6.x - 6.6) < epsilon)\n"
                "assert(math.abs(v6.y - 4.25) < epsilon)\n"
                "assert(math.abs(v6.z - 28.0) < epsilon)\n"
                "local v7 = v6 / {3.0, 2.0, 7.0}\n"
                "assert(math.abs(v7.x - 2.2) < epsilon)\n"
                "assert(math.abs(v7.y - 2.125) < epsilon)\n"
                "assert(math.abs(v7.z - 4.0) < epsilon)\n"
                "local v8 = v7:multiplyByNumber(0.5)\n"
                "assert(math.abs(v8.x - 1.1) < epsilon)\n"
                "assert(math.abs(v8.y - 1.0625) < epsilon)\n"
                "assert(math.abs(v8.z - 2.0) < epsilon)\n"
                "local v9 = v8:addNumber(3.0)\n"
                "assert(math.abs(v9.x - 4.1) < epsilon)\n"
                "assert(math.abs(v9.y - 4.0625) < epsilon)\n"
                "assert(math.abs(v9.z - 5.0) < epsilon)\n"
                "local v10 = v9:subtractNumber(2.0)\n"
                "assert(math.abs(v10.x - 2.1) < epsilon)\n"
                "assert(math.abs(v10.y - 2.0625) < epsilon)\n"
                "assert(math.abs(v10.z - 3.0) < epsilon)\n";

            auto err = luanatic::execute(state, vec3Tests);
            if (err)
                printf("%s\n", err.message().cString());

            EXPECT(!err);
        }
        lua_close(state);
    },
    SUITE("Color Tests")
    {
        lua_State * state = createLuaState();
        {
            openStandardLibraries(state);
            initialize(state);
            registerCrunch(state);

            String ctests =
                "local epsilon = 0.000001\n"
                "local col = ColorRGBA.fromNumbers(0.1, 0.2, 0.3, 0.75)\n"
                "assert(math.abs(col.r - 0.1) < epsilon)\n"
                "assert(math.abs(col.g - 0.2) < epsilon)\n"
                "assert(math.abs(col.b - 0.3) < epsilon)\n"
                "assert(math.abs(col.a - 0.75) < epsilon)\n";
                // "local col2 = ColorHSBA.fromNumbers(0.1, 0.2, 0.3, 0.75)\n";

            auto err = luanatic::execute(state, ctests);
            if (err)
                printf("%s\n", err.message().cString());

            EXPECT(!err);
        }
        lua_close(state);
    }
};

int main(int _argc, const char * _args[])
{
    return runTests(spec, _argc, _args);
}
