# Panelizer Variant

This module adds the functionality to have Panelizer Variants based off of View Modes.  What this does is allows you to have multiple layouts for one content type based off a field selection.  Right now it just works with taxonomy ter mreference, more can be added later on.  

## In order to use this, do the following:
1. Enable the module, make sure your node_view template in page manager is enabled as well.  Should be if you are using Panelizer.
2. Go to the Panelizer Variant Config Screen: admin/config/content/panelizer_variant
3. add your view modes you want to use on your content type
4. Go to your content type, click manage display tab
5. On the default tab, you will see custom display settings, click that down
6. Select your view modes you want to use (I prefer to make them like the taxonomy term you are using)
7. Go to your panelizer page: admin/config/content/panelizer
8. Select the check boxes for your content type & make sure you select these 2 options on the view modes: PANELIZE & PROVIDE DEFAULT PANEL
9. After saving, go back up and hit the context link for your view mode
10. Select a Variant and Select your Context setting
11. You can set up your layout and content now.
12. Go look at your node and this all should be happy.