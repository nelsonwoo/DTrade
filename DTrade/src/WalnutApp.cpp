#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Timer.h"

#include "AppContext.h"

Walnut::Timer g_timer{};


class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Debug");
		{
			ImGui::SliderFloat("ambience control", &m_ambient, 0.0f, 1.0f);
		}
		ImGui::End();

		ImGui::Begin("Products");
		{
			//ImGui::HelpMarker("Type B for Buy\nType S for Sell");
			if (ImGui::BeginListBox("##Products", ImVec2(-FLT_MIN, -FLT_MIN)))
			{
				static size_t item_current_idx = 0; // Here we store our selection data as an index.
				for (size_t z = 0; z < AppContext::s_productList.size(); ++z)
				{
					const bool is_selected = (item_current_idx == z);
					if (ImGui::Selectable(AppContext::s_productList[z].c_str(), is_selected))
						item_current_idx = z;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}
		}
		ImGui::End();


		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
		ImGui::Begin("Bookmarks");
		{
			if (ImGui::BeginTable("##Bookmarks", 3, flags))
			{
				// Display headers so we can inspect their interaction with borders.
				// (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)
				ImGui::TableSetupColumn("State");
				ImGui::TableSetupColumn("Id");
				ImGui::TableSetupColumn("Ticker");
				ImGui::TableHeadersRow();

				for (size_t z = 0; z < AppContext::s_bookmarkList.size(); ++z)
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::TextUnformatted("OPEN");
					ImGui::TableNextColumn();
					ImGui::TextUnformatted(AppContext::s_bookmarkList[z].c_str());
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("20136");
				}
				ImGui::EndTable();
			}
		}
		ImGui::End();


		ImGui::Begin("Price Depth");
		{
			ImGui::LabelText("##Price Depth Series ID", "Id = %s", AppContext::s_selectedProduct->id.c_str());
			ShowPriceDepth();
		}
		ImGui::End();


		static int order_prc = 100;
		static int order_qty = 0;
		ImGui::Begin("Place Order Style A");
		{
			ShowProductIdInput(AppContext::s_selectedProductId);

			// Order Prc
			ShowPrcSlider(order_prc); ImGui::SameLine(); ShowPrcInput(order_prc, "##Prc");
			// Order Qty
			ShowQtySlider(order_qty); ImGui::SameLine(); ShowQtyInput(order_qty, "##Qty");

			// BUY SELL Buttons
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 30.0f, 30.0f }); // Push item spacing
			ImGui::Spacing(); // Insert veritical spacing
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4Amb(GreenH));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Amb(GreenM));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4Amb(GreenL));
			ImGui::Button("BUY", ImVec2(139.0f, 44.0f));
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4Amb(RedH));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Amb(RedM));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4Amb(RedL));
			ImGui::Button("SELL", ImVec2(139.0f, 44.0f));
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar(); // pop item spacing
		}
		ImGui::End();


		static bool is_buying = true;
		ImGui::Begin("Place Order Style B");
		{
			ShowProductIdInput(AppContext::s_selectedProductId);

			// BUY SELL Buttons
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 6.0f }); // Push item spacing
			if (is_buying) {
				ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4Amb(GreenH));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Amb(GreenM));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4Amb(GreenL));
				if (ImGui::Button("BUY", ImVec2(178.0f, 36.0f))) {
					is_buying = true;
				}
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4Amb(GrayH));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Amb(GrayM));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4Amb(GrayL));
				if (ImGui::Button("SELL", ImVec2(130.0f, 30.f))) {
					is_buying = false;
				}
				ImGui::PopStyleColor(3);
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4Amb(GrayH));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Amb(GrayM));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4Amb(GrayL));
				if (ImGui::Button("BUY", ImVec2(130.0f, 30.0f))) {
					is_buying = true;
				}
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4Amb(RedH));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Amb(RedM));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4Amb(RedL));
				if (ImGui::Button("SELL", ImVec2(178.0f, 36.f))) {
					is_buying = false;
				}
				ImGui::PopStyleColor(3);
			}
			ImGui::PopStyleVar(); // pop item spacing

			// Order Prc
			ShowPrcSlider(order_prc); ImGui::SameLine(); ShowPrcInput(order_prc, "##Prc");
			// Order Qty
			ShowQtySlider(order_qty); ImGui::SameLine(); ShowQtyInput(order_qty, "##Qty");

			// Confirm Button
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 30.0f, 30.0f }); // Push item spacing
			ImGui::Spacing(); // Insert veritical spacing
			if (is_buying) {
				ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4Amb(GreenH));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Amb(GreenM));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4Amb(GreenL));
				ImGui::Button("Confirm BUY", ImVec2(308.0f, 44.0f));
				ImGui::PopStyleColor(3);
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4Amb(RedH));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4Amb(RedM));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4Amb(RedL));
				ImGui::Button("Confirm SELL", ImVec2(308.0f, 44.0f));
				ImGui::PopStyleColor(3);
			}
			ImGui::PopStyleVar(); // pop item spacing

		}
		ImGui::End();

		ImGui::Begin("Place Order Style C");
		{
			struct TextFilters
			{
				static int FilterBS(ImGuiInputTextCallbackData* data)
				{
					if (data->EventChar < 256 && strchr("BS", (char)data->EventChar))
						return 0;
					return 1;
				}

				static int FilterConfirm(ImGuiInputTextCallbackData* data)
				{
					if (data->EventChar < 256 && strchr("$", (char)data->EventChar))
						return 0;
					return 1;
				}
			};

			static char txtBuySell[2] = "";
			static char txtConfirm[2] = "";
			ImGui::PushItemWidth(200);
			{
				// Id
				ShowProductIdInput(AppContext::s_selectedProductId);
				
				// Buy or Sell
				ImGui::InputText("##B or S", txtBuySell, 2, ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterBS);
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4.0f, 4.0f }); // Push item spacing
				ImGui::SameLine();
				if (txtBuySell[0] == 'B') {
					ImGui::TextColored(ImVec4Amb(GreenH), "BUY");
				}
				else if (txtBuySell[0] == 'S') {
					ImGui::TextColored(ImVec4Amb(RedH), "SELL");
				}
				else {
					ImGui::Text("B or S");
				}
				ImGui::PopStyleVar();

				// Prc
				ShowPrcInput(order_prc);
				// Qty
				ShowQtyInput(order_qty);
				// Go!
				ImGui::InputText("##Go", txtConfirm, 2, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterConfirm);
				ImGui::SameLine();
				if (order_qty > 0 && txtBuySell[0] == 'B') {
					ImGui::TextColored(ImVec4Amb(GreenH), "Type '$' to BUY immediately.");
				}
				else if (order_qty > 0 && txtBuySell[0] == 'S') {
					ImGui::TextColored(ImVec4Amb(RedH), "Type '$' to SELL immediately.");
				}
				else {
					ImGui::Text("");
				}
			}
			ImGui::PopItemWidth();

			if (txtConfirm[0] == '$') {
				if (order_qty > 0 && (txtBuySell[0] == 'B' || txtBuySell[0] == 'S')) {
					ImGui::LabelText("##Debug", "Send the order here");
					order_qty = 0;
				}
				txtConfirm[0] = '\0';
			}
		}
		ImGui::End();

		ImGui::ShowDemoWindow();
	}

private:
	void ShowProductIdInput(char* txtSelectedProductId) {
		ImGui::PushItemWidth(200);
		ImGui::InputText("Id", txtSelectedProductId, 33, ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank);
		ImGui::PopItemWidth();
	}
	void ShowPriceDepth() {
		float temp_ambient{ m_ambient };
		m_ambient = 1.0f;
		if (ImGui::BeginTable("##ASK", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Prc @");
			ImGui::TableSetupColumn("Qty x");
			ImGui::TableHeadersRow();
			for (size_t z = AppContext::s_selectedProduct->priceDepth[1].size() - 1;
				z < AppContext::s_selectedProduct->priceDepth[1].size();
				--z) //loop ok: unsigned backward
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4Amb(RedH));
				ImGui::TextUnformatted(AppContext::s_selectedProduct->priceDepth[1][z].prc.c_str());
				ImGui::PopStyleColor();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(AppContext::s_selectedProduct->priceDepth[1][z].qty.c_str());
			}
			ImGui::EndTable();
		}
		if (ImGui::BeginTable("##LAST", 3, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(1);
			// TO-DO:
			// - change to the real thing
			switch (g_timer.ElapsedSecs() % 3) {
			case 0:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4Amb(RedH));
				break;
			case 1:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4Amb(GrayH));
				break;
			default:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4Amb(GreenH));
				break;
			}
			ImGui::TextUnformatted(AppContext::s_selectedProduct->ticker.prc.c_str());
			ImGui::PopStyleColor();
			ImGui::EndTable();
		}
		if (ImGui::BeginTable("##BID", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			for (size_t z = 0; z < AppContext::s_selectedProduct->priceDepth[0].size(); ++z)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4Amb(GreenH));
				ImGui::TextUnformatted(AppContext::s_selectedProduct->priceDepth[0][z].prc.c_str());
				ImGui::PopStyleColor();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(AppContext::s_selectedProduct->priceDepth[0][z].qty.c_str());
			}
			ImGui::EndTable();
		}
		m_ambient = temp_ambient;
	}
	void ShowPrcSlider(int& order_prc, const char* label = "Prc @") {
		ImGui::PushItemWidth(100);
		// TO-DO:
		// - get the literals from the selected product
		ImGui::SliderIntPrice("##Order Prc Slider", &order_prc, 5, -15000, 15000, 2, label);
		ImGui::PopItemWidth();
	}
	void ShowQtySlider(int& order_qty, const char* label = "Qty x") {
		ImGui::PushItemWidth(100);
		ImGui::SliderInt("##Order Qty Slider", &order_qty, 1, 100, label);
		ImGui::PopItemWidth();
	}
	void ShowPrcInput(int& order_prc, const char* label = "Prc @") {
		ImGui::PushItemWidth(200);
		ImGui::InputIntPrice(label, &order_prc, 5, 2); // values of 5-point steps 2 decimals
		ImGui::PopItemWidth();
	}
	void ShowQtyInput(int& order_qty, const char* label = "Qty x") {
		ImGui::PushItemWidth(200);
		// TO-DO:
		// - get the literals from the selected product
		ImGui::InputInt(label, &order_qty, 1, 100);
		ImGui::PopItemWidth();
		order_qty = order_qty < 0 ? 0 : order_qty;
		if (order_qty > 100) {
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4.0f, 4.0f }); // Push item spacing
			ImGui::SameLine();
			ImGui::TextColored(ImVec4Amb(WarningH), "(WARNING: quite many!)");
			ImGui::PopStyleVar();
		}
	}
	float m_ambient{ 1.0f };
	ImVec4 ImVec4Amb(const ImVec4& c) {
		return ImVec4{ c.x, c.y, c.z, m_ambient };
	}
	ImVec4 RedH{ 0.85f, 0.0f, 0.35f, 1.0f };
	ImVec4 RedM{ 0.75f, 0.0f, 0.30f, 1.0f };
	ImVec4 RedL{ 0.65f, 0.0f, 0.25f, 1.0f };
	ImVec4 GreenH{ 0.40f, 0.80f, 0.0f, 1.0f };
	ImVec4 GreenM{ 0.35f, 0.70f, 0.0f, 1.0f };
	ImVec4 GreenL{ 0.30f, 0.60f, 0.0f, 1.0f };
	ImVec4 GrayH{ 0.7f, 0.7f, 0.7f, 1.0f };
	ImVec4 GrayM{ 0.6f, 0.6f, 0.6f, 1.0f };
	ImVec4 GrayL{ 0.5f, 0.5f, 0.5f, 1.0f };
	ImVec4 WarningH{ 1.0f, 0.5f, 0.0f, 1.0f };
	ImVec4 WarningM{ 1.0f, 0.4f, 0.0f, 1.0f };
	ImVec4 WarningL{ 1.0f, 0.3f, 0.0f, 1.0f };
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "DTrade";

	AppContext::s_theProduct = {
		"The Product",
		{
			{
				{"20055","30"},
				{"20053","32"},
				{"20042","76"},
				{"20038","115"},
				{"20027","120"}
			},
			{
				{"20056","30"},
				{"20058","32"},
				{"20063","27"},
				{"20071","93"},
				{"20079","110"}
			}
		},
		{
			"20056", "3", AppContext::PriceTrend::Flat
		}
	};

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}