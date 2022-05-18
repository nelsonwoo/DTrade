#pragma once

#include <vector>
#include <string>


namespace AppContext {

	struct PrcQty {
		std::string prc;
		std::string qty;
	};

	enum class PriceTrend {
		Up, Down, Flat
	};

	extern std::vector<std::string> s_productList;
	extern std::vector<std::string> s_bookmarkList;
	extern std::vector<PrcQty> s_priceDepthBuy;
	extern std::vector<PrcQty> s_priceDepthSell;
	extern PriceTrend s_tickerPrice_e;
	extern std::string s_tickerPrice;

}