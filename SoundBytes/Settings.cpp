#include "pch.h"
#include "SoundBytes.h"

void SoundBytes::RenderSettings() {
	ImGui::TextUnformatted("A plugin to play sound bytes when things happen during the game");

	CVarWrapper enableSounds = cvarManager->getCvar("soundbytes_enabled");
	if (!enableSounds) { return; }
	bool enabled = enableSounds.getBoolValue();
	if (ImGui::Checkbox("Enable Plugin", &enabled)) {
		enableSounds.setValue(enabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable or disable the Sound Bytes plugin");
	}

	ImGui::TextUnformatted("Volume of the sound");
	CVarWrapper carHitVolume = cvarManager->getCvar("soundbytes_engine_volume");
	if (!carHitVolume) { return; }
	int volume = carHitVolume.getIntValue();
	if (ImGui::InputInt("Volume", &volume)) {
		carHitVolume.setValue(volume);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Volume from 0 to 100");
	}

	ImGui::TextUnformatted("Path of sound to play when the ball touches a car");
	CVarWrapper carHitSound = cvarManager->getCvar("soundbytes_car_hit_path");
	if (!carHitSound) { return; }
	std::string carHit = carHitSound.getStringValue();
	if (ImGui::InputText("Car Hit Sound", &carHit)) {
		carHitSound.setValue(carHit);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Remove quotations, must be exact path with the .extension included!");
	}

	ImGui::TextUnformatted("Minumum speed of ball when hit to play sound");
	CVarWrapper carHitSpeed = cvarManager->getCvar("soundbytes_car_hit_speed");
	if (!carHitSpeed) { return; }
	int speed = carHitSpeed.getIntValue();
	if (ImGui::InputInt("Car Hit Speed", &speed)) {
		carHitSpeed.setValue(speed);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Speed in MPH");
	}
}