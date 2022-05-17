#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "AppContext.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Products");
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
		ImGui::End();


		static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

		ImGui::Begin("Bookmarks");
		if (ImGui::BeginTable("##Bookmarks", 3, flags))
		{
			// Display headers so we can inspect their interaction with borders.
			// (Headers are not the main purpose of this section of the demo, so we are not elaborating on them too much. See other sections for details)
			ImGui::TableSetupColumn("State");
			ImGui::TableSetupColumn("ID");
			ImGui::TableSetupColumn("Last Price");
			ImGui::TableHeadersRow();

			for (size_t z = 0; z < AppContext::s_bookmarkList.size(); z++)
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
		ImGui::End();

		static float order_prc = 0.0f;
		static int order_qty = 0;
		ImGui::Begin("Place Order");

		// TO-DO:
		ImGui::LabelText("##Order Series ID", "Id = %s", "PRODUCT_02");

		// Input boxes
		// TO-DO:
		// - make price increment fixed like DragFloat.
		// - make boxes full width/
		ImGui::SliderFloat("##Order Price", &order_prc, 15000.0f, 35000.0f, "Prc = %.0f");
		ImGui::SliderInt("##Order Quantity", &order_qty, 1, 100, "Qty = %d");

		// BUY SELL Buttons
		// TO-DO:
		// - make the buttons bigger.
		// - insert some gap between the buttons.
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.0f, 0.5f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.0f, 0.8f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.0f, 0.4f, 0.0f));
		ImGui::Button("BUY");
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.5f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.8f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.4f, 0.0f, 0.0f));
		ImGui::Button("SELL");
		ImGui::PopStyleColor(3);

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