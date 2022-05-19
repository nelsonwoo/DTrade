#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Timer.h"

#include "AppContext.h"

Walnut::Timer g_timer;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Products");
		{
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
			ImGui::LabelText("##Price Depth Series ID", "Id = %s", "PRODUCT_02");
			if (ImGui::BeginTable("ASK", 2, flags))
			{
				ImGui::TableSetupColumn("Prc");
				ImGui::TableSetupColumn("Qty");
				ImGui::TableHeadersRow();
				for (size_t z = AppContext::s_priceDepthSell.size() - 1; z < AppContext::s_priceDepthSell.size(); --z) //loop ok: unsigned backward
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(0.5f, 0.0f, 0.0f));
					ImGui::TextUnformatted(AppContext::s_priceDepthSell[z].prc.c_str());
					ImGui::PopStyleColor();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted(AppContext::s_priceDepthSell[z].qty.c_str());
				}
				ImGui::EndTable();
			}
			if (ImGui::BeginTable("LAST", 3, flags))
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(1);
				switch (g_timer.ElapsedSecs() % 3) {
				case 0:
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(0.2f, 0.7f, 0.2f));
					break;
				case 1:
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(0.7f, 0.2f, 0.2f));
					break;
				default:
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(0.6f, 0.6f, 0.6f));
					break;
				}
				ImGui::TextUnformatted(AppContext::s_tickerPrice.c_str());
				ImGui::PopStyleColor();
				ImGui::EndTable();
			}
			if (ImGui::BeginTable("BID", 2, flags))
			{
				for (size_t z = 0; z < AppContext::s_priceDepthBuy.size(); ++z)
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::ImColor(0.0f, 0.5f, 0.0f));
					ImGui::TextUnformatted(AppContext::s_priceDepthBuy[z].prc.c_str());
					ImGui::PopStyleColor();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted(AppContext::s_priceDepthBuy[z].qty.c_str());
				}
				ImGui::EndTable();
			}
		}
		ImGui::End();



		static float order_prc_f = -100.0f;
//		static float order_prc_multiplier = 0.01f;
		static int order_prc_point = -100;
		static int order_qty = 0;
		ImGui::Begin("Place Order Style A");
		{
			ImGui::LabelText("##Order Series ID", "Id = %s", "PRODUCT_02");

			ImGui::BeginGroup();
			{
				// Order Prc
				ImGui::PushItemWidth(100);
				ImGui::SliderIntPrice("##Order Prc Slider", &order_prc_point, 5, -15000, 15000, 2, "@ Prc");
				order_prc_f = 0.01f * order_prc_point;
				ImGui::PopItemWidth();
				/*
				ImGui::PushItemWidth(100);
				ImGui::SliderInt("##Order Prc Slider", &order_prc_point, 15000, 35000, "@ Prc %d");
				ImGui::PopItemWidth();
				*/
				// TO-DO:
				// - Create InputPrice function with steps
				ImGui::SameLine();
				ImGui::PushItemWidth(200);
				ImGui::InputFloat("##Order Prc Input", &order_prc_f, 0.05f, 0.05f, "%.2f");
				order_prc_point = static_cast<int>(order_prc_f / 0.01f);
				ImGui::PopItemWidth();
				
				// Order Qty
				ImGui::PushItemWidth(100);
				ImGui::SliderInt("##Order Qty Slider", &order_qty, 1, 100, "x Qty");
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::PushItemWidth(200);
				ImGui::InputInt("##Order Qty Input", &order_qty, 1, 100);
				ImGui::PopItemWidth();

				ImGui::LabelText("order_prc_point", "%d", order_prc_point);
			}
			ImGui::EndGroup();
			ImVec2 size = ImGui::GetItemRectSize(); // want to know the current size of this group

			// BUY SELL Buttons
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 30.0f, 30.0f }); // Push item spacing
			ImGui::Spacing(); // Insert veritical spacing
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.0f, 0.8f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.0f, 0.9f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.0f, 0.7f, 0.0f));
			ImGui::Button("BUY", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, size.y));
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.8f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.9f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.7f, 0.0f, 0.0f));
			ImGui::Button("SELL", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, size.y));
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar(); // pop item spacing
		}
		ImGui::End();


		static bool is_buying = true;
		ImGui::Begin("Place Order Style B");
		{
			ImGui::LabelText("##Order Series ID", "Id = %s", "PRODUCT_02");

			// BUY SELL Buttons
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 6.0f }); // Push item spacing
			if (is_buying) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.0f, 0.8f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.0f, 0.8f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.0f, 0.6f, 0.0f));
				if (ImGui::Button("BUY", ImVec2(178.0f, 36.0f))) {
					is_buying = true;
				}
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.4f, 0.4f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.6f, 0.4f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.6f, 0.4f, 0.4f));
				if (ImGui::Button("SELL", ImVec2(130.0f, 30.f))) {
					is_buying = false;
				}
				ImGui::PopStyleColor(3);
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.4f, 0.4f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.4f, 0.6f, 0.4f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.4f, 0.6f, 0.4f));
				if (ImGui::Button("BUY", ImVec2(130.0f, 30.0f))) {
					is_buying = true;
				}
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.8f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.8f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.6f, 0.0f, 0.0f));
				if (ImGui::Button("SELL", ImVec2(178.0f, 36.f))) {
					is_buying = false;
				}
				ImGui::PopStyleColor(3);
			}
			ImGui::PopStyleVar(); // pop item spacing

			ImGui::BeginGroup();
			{
				// Order Prc
				// TO-DO:
				// - Store value as int but display with decimal.
				/*
				ImGui::PushItemWidth(100);
				ImGui::SliderInt("##Order Prc B Slider", &order_prc_point, 15000, 35000, "@ Prc");
				order_prc_f = order_prc_multiplier * order_prc_point;
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::PushItemWidth(200);
				ImGui::InputFloat("##Order Prc B Input", &order_prc_f, order_prc_multiplier, order_prc_multiplier, "%.2f");
				order_prc_point = static_cast<int>((order_prc_f + 0.000001f) / order_prc_multiplier);
				ImGui::PopItemWidth();
				*/
				// Order Qty
				ImGui::PushItemWidth(100);
				ImGui::SliderInt("##Order Qty B Slider", &order_qty, 1, 100, "x Qty");
				ImGui::PopItemWidth();
				ImGui::SameLine();
				ImGui::PushItemWidth(200);
				ImGui::InputInt("##Order Qty B Input", &order_qty, 1, 100);
				ImGui::PopItemWidth();
			}
			ImGui::EndGroup();
			ImVec2 size = ImGui::GetItemRectSize(); // want to know the current size of this group

			// Confirm Button
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 30.0f, 30.0f }); // Push item spacing
			ImGui::Spacing(); // Insert veritical spacing
			if (is_buying) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.0f, 0.8f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.0f, 0.9f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.0f, 0.7f, 0.0f));
				ImGui::Button("Confirm BUY", ImVec2(size.x, size.y));
				ImGui::PopStyleColor(3);
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.8f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.9f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.7f, 0.0f, 0.0f));
				ImGui::Button("Confirm SELL", ImVec2(size.x, size.y));
				ImGui::PopStyleColor(3);
			}
			ImGui::PopStyleVar(); // pop item spacing

		}
		ImGui::End();

		ImGui::ShowDemoWindow();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "DTrade";

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