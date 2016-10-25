#ifndef __LUA_RAPIDJSON_USERDATA_HPP__
#define __LUA_RAPIDJSON_USERDATA_HPP__

#include "lua.hpp"

template <typename T>
struct Userdata {
	static int create(lua_State* L) {
		push(L, construct(L));
		return 1;
	}

	static T* construct(lua_State* L);

	static void push(lua_State* L, T* c) {
		if (c == NULL) {
			lua_pushnil(L);
			return;
		}
		T** ud = reinterpret_cast<T**>(L, sizeof(*ud));
		if (!ud) {
			luaL_error(L, "Out of memory");
		}

		*ud = c;

		lua_getfield(L, LUA_REGISTRYINDEX, Metatable);
		lua_setmetatable(L, -2);
	}

	static void setupMetatable(lua_State* L, int idx) {
		lua_pushvalue(L, idx);

		lua_pop(L, 1);
	}

	static T** getUserdata(lua_State* L, int idx) {
		return reinterpret_cast<T**>(lua_touserdata(L, idx));
	}


	static T* get(lua_State* L, int idx) {
		T** p = getUserdata(L, idx);
		if (p != NULL && *p != NULL) {
			if (lua_getmetatable(L, idx)) {  /* does it have a metatable? */
				lua_getfield(L, LUA_REGISTRYINDEX, Metatable);  /* get correct metatable */
				if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
					lua_pop(L, 2);  /* remove both metatables */
					return *p;
				}
			}
		}
		return NULL;
	}

	static T* check(lua_State* L, int idx) {
		T** ud = reinterpret_cast<T**>(luaL_checkudata(L, idx, Metatable));
		if (!*ud)
			luaL_error(L, "%s already closed", Metatable);
		return *ud;
	}

	static int metamethod_gc(lua_State* L) {
		T** ud = reinterpret_cast<T**>(luaL_checkudata(L, 1, Metatable));
		if (*ud) {
			delete *ud;
			*ud = NULL;
		}
		return 0;
	}

	static int metamethod_tostring(lua_State* L) {
		T** ud = getUserdata(L, 1);
		if (*ud) {
			lua_pushfstring(L, "%s (%p)", Metatable, *ud);
		}
		else {
			lua_pushfstring(L, "%s (closed)", Metatable);
		}
		return 1;
	}

	static const char* const Metatable;
};


#endif //__LUA_RAPIDJSON_USERDATA_HPP__