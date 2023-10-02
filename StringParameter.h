#pragma once
#include <array>
#include <string_view>
#include <ostream>

/// @brief 文字の型かどうかを判定するコンセプト
///	@tparam Type 判定する型
template <typename Type>
concept CharacterType =
std::is_same_v<char, Type> ||
std::is_same_v<wchar_t, Type> ||
std::is_same_v<char8_t, Type> ||
std::is_same_v<char16_t, Type> ||
std::is_same_v<char32_t, Type>;

/// @brief コンパイル時文字列を扱うクラステンプレート
/// @tparam CharType 文字の型
/// @tparam Size サイズ
template <CharacterType CharType, size_t Size>
class StringParameter :
	public std::array<CharType, Size>
{
	static_assert(Size > 0, "サイズは1以上である必要があります");
public:
	/// @brief コンストラクタ
	/// @param data サイズ付き文字列のポインタ
	constexpr StringParameter(const CharType(&data)[Size])
	{
		std::copy(data, data + Size, this->begin());
		this->back() = CharType();
	}

	/// @brief std::basic_string_viewに対するキャスト演算子
	constexpr operator std::basic_string_view<CharType>() const
	{
		return std::basic_string_view<CharType>(this->data(), Size);
	}
};

/// @brief StringParameterを出力するための演算子
/// @tparam CharType 文字の型
/// @tparam Size サイズ
/// @param os 出力先
/// @param stringParameter 出力する文字列
/// @return 出力先
template<CharacterType CharType, size_t Size>
std::ostream& operator<<(std::ostream& os, const StringParameter<CharType, Size>& stringParameter)
{
	return os.write(stringParameter.data(), stringParameter.size());
}

//クラステンプレートを使用した昔の書き方
//template <StringParameter Name1, StringParameter Name2>
//class IsSameString : public std::false_type
//{
//};
//
//template <StringParameter Name>
//class IsSameString<Name, Name> : public std::true_type
//{
//};

/// @brief String1とString2が同じ文字列かどうかを判定する
/// @tparam String1 文字列1
/// @tparam String2 文字列2
template <StringParameter String1, StringParameter String2>
constexpr bool isSameStringV = false;

/// @brief isSameStringVに同じ文字列を渡した場合の特殊化
/// @tparam String 文字列
template <StringParameter String>
constexpr bool isSameStringV<String, String> = true;

/// @brief 文字列リストの中に重複がないかどうかを判定する
/// @tparam String 比較対象の文字列
/// @tparam Strings 他の文字列
template <StringParameter String, StringParameter... Strings>
constexpr bool isStringSetV = (!isSameStringV<String, Strings> && ...) && isStringSetV<Strings...>;

/// @brief isStringSetVに一つしか文字列を渡さなかった場合の特殊化
/// @tparam String 文字列
template <StringParameter String>
constexpr bool isStringSetV<String> = true;

