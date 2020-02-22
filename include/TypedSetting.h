#pragma once

template<typename ScopedEnum, ScopedEnum ScopedEnumValue>
struct TypedSetting { using type = void; };

template<auto ScopedEnumValue> using TypedSetting_t = typename TypedSetting<decltype(ScopedEnumValue), ScopedEnumValue>::type;
