Spout-AfterEffects
==================

The After Effects plugin for sending textures to Spout enabled apps.

### Installation
- SpoutSender.aex should be placed in the Plug-ins folder of After Effects
- If you get an error when starting AE, you should either place the 2 .dll files in the root folder of After Effects (the folder containing AfterFX.exe), or place them in a separate folder and add this folder to the PATH environment variable.
- Once AE is loaded, the plugin start sharing as soon as you open a composition as with sender's name "AE". 
- Spout restarts a texture sharing each time you switch between compositions or resize the current composition. It may break some receivers like FFGL (Resolume) which don't like having several texture sharing with the same sender name and different sizes.

### Notes
- The plugin has been compiled against the After Effects CC SDK, and has not been tested with other versions than AE CC.
- This plugin is in a very early stage and has been compiled with a modified version of Spout SDK, and may cause crashes.
- I recommend you to use the plugin only when you want to use the Spout feature or to test it, and to remove the file from the AE Plug-in folder once you're done.

Spout is currently going through a lot of work, so this plugin may not work when Spout gets updated.
I put the plugin by popular request, but i won't guarantee any support of it until Spout is in a more mature state.

Thank you

This project is done on my spare time, if you feel like it you can donate whatever you want !
<a href="https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=bkuperberg%40hotmail%2ecom&lc=US&item_name=Ben%20Kuper&item_number=open_paypal_donate&currency_code=EUR&bn=PP%2dDonationsBF%3abtn_donate_LG%2egif%3aNonHosted"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif" /></a>

