#include "AppContext.h"

namespace AppContext {


	std::vector<std::string> s_productList = { "PRODUCT_01", "PRODUCT_02", "PRODUCT_03", "PRODUCT_04" };

	std::vector<std::string> s_bookmarkList = { "PRODUCT_02", "PRODUCT_03" };
/*

	std::vector<PrcQty> s_priceDepthBuy = {
		{"20055","30"},
		{"20053","32"},
		{"20042","76"},
		{"20038","115"},
		{"20027","120"}
	};
	std::vector<PrcQty> s_priceDepthSell = {
		{"20056","30"},
		{"20058","32"},
		{"20063","27"},
		{"20071","93"},
		{"20079","110"}
	};
	std::string s_selectedProductId = "PRODUCT_02";
	std::string s_tickerPrice = "20056";
	PriceTrend s_tickerPrice_e = PriceTrend::Flat;
	*/
	
	ProductPresentation s_theProduct;
	ProductPresentation* s_selectedProduct{ &s_theProduct };;
	char s_selectedProductId[33]{};
}