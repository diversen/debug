<?php

/**
 * block file is used for showing content categories
 *
 * @package content
 */

//include_module ('google_contact');
/**
 * function for showing a category block
 *
 * @return string   category block as html
 */
function block_debug(){
    //$event = new event(false);
    ob_start();
    //$info = moduleLoader::getModuleIniSettings('google_contact');
    print "<a name=\"debug_block\">Debug block</a><br />";
    print "<ul>\n";
    print "<li>" . create_link($_SERVER['REQUEST_URI'] . "#benchmark", " Benchmark") . "<br />";
    print "<li>" . create_link($_SERVER['REQUEST_URI'] . "#main_settings", " Main Settings") . "<br />";
    print "<li>" . create_link($_SERVER['REQUEST_URI'] . "#module_settings", " Module Settings") . "<br />";
    print "<li>" . create_link($_SERVER['REQUEST_URI'] . "#sql", " SQL") . "<br />";
    print "<li>" . create_link($_SERVER['REQUEST_URI'] . "#server", " Server Info") . "<br />";
    print "</ul>\n";
    //print $info['info'];
    //get_module_ini('info');

    //$event->displayEventsShort();
    $str = ob_get_contents ();
    ob_end_clean();
    return $str;
}
