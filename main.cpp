#include <iostream>
#include <tuple>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
	if (2 != argc)
	{
		std::cout << R"(Need A Date Type. ex) "2021-11-22-" , "21.11.22." ...)" << std::endl;
		return EXIT_FAILURE;
	}

	auto extrac_date = [](const std::string& _date) -> std::tuple<int, std::vector<std::size_t>, std::vector<char>>
	{
		auto length_date = _date.length();

		std::vector<std::size_t> id_digits;
		std::vector<char> divider_date;

		for (std::size_t i = 0; i < length_date; ++i)
		{
			if (isdigit(_date[i]))
			{
				id_digits.emplace_back(i);
			}
			else
			{
				divider_date.emplace_back(_date[i]);
			}
		}

		return { length_date, id_digits, divider_date };
	};

	std::string date_type = argv[1];

	auto [length_date, id_digits, divider_date] = extrac_date(date_type);

	auto set_date_front = [length_date, id_digits, divider_date](const std::string& _date) -> std::string
	{
		if (_date.size() <= length_date)
			return _date;

		auto comparision_index = 0;
		auto divider_index = 0;
		auto last_index_date = 0;

		for (auto c : _date)
		{
			if ( (id_digits.end() != std::find(id_digits.begin(), id_digits.end(), comparision_index))
				&& isdigit(c) )
			{
				++comparision_index;
			}
			else if ( (id_digits.end() == std::find(id_digits.begin(), id_digits.end(), comparision_index))
					&& (divider_date.size() > divider_index)
					&& (divider_date[divider_index] == c) )
			{
				++comparision_index;
				++divider_index;
			}
			else
			{
				comparision_index = 0;
				divider_index = 0;
			}

			++last_index_date;

			if (comparision_index == length_date)
				break;
		}

		if (comparision_index != length_date)
			return _date;

		auto first_index_date = last_index_date - length_date;

		auto name_with_date_front = _date.substr(first_index_date, length_date)
									+ _date.substr(0, first_index_date)
									+ _date.substr(last_index_date);

		return name_with_date_front;
	};

	fs::path p = fs::current_path();

	for (auto& sub : fs::directory_iterator(p))
	{
		if (!sub.is_directory())
			continue;

		auto dir_name = sub.path().filename().string();

		auto new_dir_name = p.string() + "\\" + set_date_front(dir_name);

		fs::rename(sub.path(), new_dir_name);
	}

	return EXIT_SUCCESS;
}
