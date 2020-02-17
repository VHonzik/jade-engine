#pragma once

#include "IScene.h"

#include <array>
#include <vector>
#include <unordered_map>

namespace JadeEngine
{
  class Slider;
  class Button;
  class Text;
  class Checkbox;
  class Dropdown;
  class TransformGroup;

  class OptionsMenuScene : public IScene
  {
  public:
    OptionsMenuScene();
    void Start() override;
    void Update() override;
  private:
    void AdjustSections();

    std::array<Button*, 3> _sectionTitles;
    int32_t _activeSection;
    int32_t _activeSectionY;
    int32_t _passiveSectionY;

    Button* _backButton;

    TransformGroup* _slidersRow;
    Slider* _musicVolume;
    Text* _musicVolumeDescription;
    Slider* _soundVolume;
    Text* _soundVolumeDescription;

    TransformGroup* _keybindColumn;
    std::vector<Text*> _keybindingDescription;
    std::vector<Button*> _keybindingButtons;
    std::vector<int32_t> _keybindingSettingsIds;
    bool _pickingKeybind;
    Button* _pickingKeybindSlot;
    int32_t _pickingKeybindIndex;

    Text* _fullScreenDescription;
    Checkbox* _fullScreenCheckbox;
    Dropdown* _resolutionsDropdown;

  };
}
