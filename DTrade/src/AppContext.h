#pragma once

#include <vector>
#include <string>


namespace AppContext {

	// to be replaced
	extern std::vector<std::string> s_productList;
	extern std::vector<std::string> s_bookmarkList;

	enum class PriceTrend {
		Up, Down, Flat
	};

	struct PrcQty {
		std::string prc{};
		std::string qty{};
		PriceTrend eTrend{ PriceTrend::Flat };
	};

	struct ProductPresentation {
		std::string id{};
		std::vector<PrcQty> priceDepth[2];
		PrcQty ticker{};
	};

	extern ProductPresentation s_theProduct; // to be replaced
	extern ProductPresentation* s_selectedProduct;
	extern char s_selectedProductId[33];
}