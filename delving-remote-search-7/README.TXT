Delving Remote Search
---------------------

This module provides access to a remote Delving Platform Search API.


Information for administrators
------------------------------


Installation Instructions
-------------------------

Place this module in your sites/all 'modules' directory. Because this is a custom module (not yet a contributed module) it
is recommended you create a "/sites/all/modules/custom" directory and place it in there. This is not required but it is
recommended as it will be clear from the directory structure which are contributed and which are custom modules

Once the module is placed you can go to the Drupal modules page and activate both the Delving API and Delving Search
modules.

Once the modules are activated go to the Delving Remote-Search Settings page and configure the API and Display


API Configuration
-----------------

- Delving Remote-Search API settings:
    -   Delving Remote-search API URL: The URL to which all queries are sent
    -   Number of rows to return in search results

- Search Constraints
    -   Return only results with digital objects: If the API URL has this value indexed, and it is set to 'true', then
        only results with digital objects will be returned
    -   Limit results by facets: Here you can enter a comma delimited list of facets on which to
        constrain the results. For example: "COUNTY:Oslo,MUNICIPALITY:Majorstuen"
        * It is recommended that if you wish to limit the queries to results from one specific COUNTY, that you do not
        activate the COUNTY facet block as this might cause some confusion for the end-user.
- Page Display Settings
    -   Search-page name value: The name of the search page as it will appear in the drupal menu system
    -   Item page link text: The text for the link back to the results from the drupal object view page.
        Defaults to 'Return to results'

- Object Display Settings
    -   Choose where you would like to view the object: This is where the title link in the result list will point to.
        It can be to the internal Drupal page for the object created by this module, the 'original context' url from the
        objects metadata field 'isShownAt', or to a Delving Platform portal instance.


Facet Blocks Configuration
--------------------------

Go to the blocks configuration page to place the search refinement facet blocks. The blocks have default titles in english.
You can click the 'configure' link for each individual block and add a new title.


Information for developers
--------------------------

If you want to change the DELVING_SEARCH_RESULTS_PAGE_PATH or the DELVING_SEARCH_ITEM_PAGE_PATH then you may find that the facets
blocks no longer show. You can either configure the facets to show on the DELVING_SEARCH_RESULTS_PAGE_PATH by clicking
on the facet configure link in the block list and set the new page path there, or you can deactivate and the uninstall the
module and activate it again. By doing this the new page path on which the facet blocks must appear will be written to
the database variables table
