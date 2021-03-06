// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab - Rice University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>

using namespace mahi::gui;
using namespace mahi::util;

class PlotDragNDropDemo : public Application {
public:
    PlotDragNDropDemo() : Application(800, 500, "Plot Drag N Drop Demo", false) {
        ImGui::DisableViewports();
        plot.x_axis.maximum  = 10;
        plot.x_axis.lock_min = true;
        plot.x_axis.lock_max = true;
    }

    void update() override {
        float t = (float)time().as_seconds();

        ImGui::SetNextWindowPos({0,0}, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize({800,500}, ImGuiCond_FirstUseEver);
        ImGui::Begin("Data List", nullptr, ImGuiWindowFlags_NoTitleBar);
        ImGui::BeginGroup();
        if (ImGui::Button("Clear Plot", {100, 0}))
            items.clear();
        for (int i = 0; i < 10; ++i) {
            std::string label = fmt::format("data_{}", i);
            ImGui::Selectable(label.c_str(), false, 0, {100, 0});
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                payload = i;
                ImGui::SetDragDropPayload("DND_PLOT", &payload, sizeof(int));
                ImGui::TextUnformatted(label.c_str());
                ImGui::EndDragDropSource();
            }
        }
        ImGui::EndGroup();

        for (auto& i : items)
            ImGui::PlotItemBufferPoint(
                i, t, i.data.back().y + random_range(0.0049f, 0.0051f) * random_range(-1.0, 1.0), 1000);

        ImGui::SameLine();
        ImGui::Plot("My Rolling Plot", plot, items);
        ImGui::PlotAxisScroll(plot.x_axis, t, 10);
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("My Plot");
            ImGui::EndTooltip();
        }
        if (ImGui::BeginDragDropTarget()) {
            if (ImGui::AcceptDragDropPayload("DND_PLOT")) {
                items.emplace_back();
                items.back().label = fmt::format("data_{}", payload);
                items.back().data.push_back({t, (float)random_range(0.25, 0.75)});
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::End();
    }

    int                          payload;
    ImGui::PlotInterface         plot;
    std::vector<ImGui::PlotItem> items;
};

int main(int argc, char const* argv[]) {
    PlotDragNDropDemo demo;
    demo.run();
    return 0;
}