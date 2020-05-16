
  function Component()
  {
      // default constructor
  }

  Component.prototype.createOperations = function()
  {
      // call default implementation to actually install
      component.createOperations();

      if (systemInfo.kernelType === "linux") {
          //Create desktop entry for SaliSynth
          component.addOperation("CreateDesktopEntry", "salisynth.desktop",
             "Version=@ProductVersion@\nType=Application\nCategories=Application;\nName=SaliSynth\nExec=@TargetDir@/SaliSynth.sh");
          //Create desktop entry for delete salicad
          component.addOperation("CreateDesktopEntry", "salisynthdel.desktop",
             "Version=@ProductVersion@\nType=Application\nCategories=Application;\nName=SaliSynth Uninstall\nExec=@TargetDir@/maintenancetool");
      }
  }

