#include <iostream>
#include "NamedTuple.h"

/// @brief 名前付きタプルの表示用関数
/// @tparam Name 表示したい値の名前
/// @tparam Containers 型リスト
/// @param tuple 名前付きタプル
template <StringParameter Name, typename... Containers>
void Print(const NamedTuple<Containers...>& tuple)
{
	std::cout << Name << ": " << get<Name>(tuple) << std::endl;
}

int main()
{
	auto tuple = make_named_tuple<"a","b","c">(1, 1.2f, 3.0);

	Print<"a">(tuple);
	Print<"b">(tuple);
	Print<"c">(tuple);
	//Print<"d">(tuple);			//"d"に対して値は設定していないためコンパイルエラー

	system("pause");
}
