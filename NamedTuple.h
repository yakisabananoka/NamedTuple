#pragma once
#include "StringParameter.h"

/// @brief 名前と型を持つクラステンプレート
/// @tparam ElemName 要素の名前
/// @tparam ElemType 要素の型
template <StringParameter ElemName, typename ElemType>
class NamedType
{
public:
	/// @brief 名前
	static constexpr auto name = ElemName;

	/// @brief 型
	using Type = ElemType;
};

/// @brief 名前付きタプル(プライマリ)
/// @tparam Containers NamedTypeのリスト
template <typename... Containers>
class NamedTuple;

/// @brief 名前付きタプル
///	details NamedTypeのリストとして展開した場合
/// @tparam Names 名前のリスト
/// @tparam Types 型のリスト
template<StringParameter... Names, typename... Types>
class NamedTuple<NamedType<Names, Types>...> :
	public std::tuple<Types...>
{
	static_assert(isStringSetV<Names...>, "名前が重複しています");
public:
	///@brief 継承コンストラクタ
	using std::tuple<Types...>::tuple;
};

/// @brief 名前から名前付きタプルの要素のインデックスと型を取得するクラステンプレート(プライマリ)
/// @tparam Index インデックス(最初から探索する場合は0を入れる)
/// @tparam Name 名前
/// @tparam Containers 型リスト
template <size_t Index, StringParameter Name, typename... Containers>
class NamedTupleLookup;

/// @brief 名前から名前付きタプルの要素のインデックスと型を取得するクラステンプレート
///	@details NamedTypeのリストとして展開した場合
///	@tparam Index インデックス(最初から探索する場合は0を入れる)
///	@tparam Name 名前
///	@tparam ElemName 最初の要素の名前
///	@tparam ElemType 最初の要素の型
///	@tparam Containers 残りの要素のリスト
template <size_t Index, StringParameter Name, StringParameter ElemName, typename ElemType, typename... Containers>
class NamedTupleLookup<Index, Name, NamedType<ElemName, ElemType>, Containers...>
{
	/// @brief 再帰的に探索
	using Next = NamedTupleLookup<Index + 1, Name, Containers...>;
public:
	/// @brief 要素の型
	using Type = typename Next::Type;

	/// @brief 要素のインデックス
	static constexpr size_t index = Next::index;
};

/// @brief 名前から名前付きタプルの要素のインデックスと型を取得するクラステンプレート
///	@details 名前が一致した場合
/// @tparam Index インデックス
/// @tparam Name 名前
/// @tparam ElemType 要素の型
/// @tparam Containers 残りの要素型のリスト
template <size_t Index, StringParameter Name, typename ElemType, typename... Containers>
class NamedTupleLookup<Index, Name, NamedType<Name, ElemType>, Containers...>
{
public:
	/// @brief 要素の型
	using Type = ElemType;

	/// @brief 要素のインデックス
	static constexpr size_t index = Index;
};

/// @brief NamedTupleの要素のインデックスを取得する変数テンプレート
/// @tparam Name 名前
/// @tparam Containers 要素型のリスト
template <StringParameter Name, typename... Containers>
constexpr size_t namedTupleIndex = NamedTupleLookup<0, Name, Containers...>::index;

/// @brief NamedTupleの要素の型を取得するエイリアステンプレート
/// @tparam Name 名前
/// @tparam Containers 要素型のリスト
template <StringParameter Name, typename... Containers>
using  NamedTupleType = typename NamedTupleLookup<0, Name, Containers...>::Type;

/// @brief 指定した名前の要素を取得する関数テンプレート
/// @tparam Name 名前
/// @tparam Containers 要素型のリスト
/// @param tuple 名前付きタプル
/// @return 要素の参照
template <StringParameter Name, typename... Containers>
NamedTupleType<Name, Containers...>& get(NamedTuple<Containers...>& tuple)
{
	return std::get<namedTupleIndex<Name, Containers...>>(tuple);
}

/// @brief 指定した名前の要素を取得する関数テンプレート
///	@details const版
/// @tparam Name 名前
/// @tparam Containers 要素型のリスト
/// @param tuple 名前付きタプル
/// @return 要素の参照
template <StringParameter Name, typename... Containers>
const NamedTupleType<Name, Containers...>& get(const NamedTuple<Containers...>& tuple)
{
	return std::get<namedTupleIndex<Name, Containers...>>(tuple);
}

/// @brief 名前付きタプルを作成するヘルパー関数テンプレート
///	@details 名前と型が一対一に対応していることを要求する
/// @tparam Names 名前のリスト
/// @tparam Types 型のリスト
/// @param args 要素を構成する値
/// @return 名前付きタプル
template <StringParameter... Names, typename... Types>
	requires (sizeof...(Names) == sizeof...(Types))
NamedTuple<NamedType<Names, Types>...> make_named_tuple(Types&&... args)
{
	return NamedTuple<NamedType<Names, Types>...>(std::forward<Types>(args)...);		//コンストラクタに対して完全転送
}
