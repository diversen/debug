<?php

/**
 * model file for printing a debug array with usefull infomation
 *
 * @package    debug
 */

// }}}
// {{{ function do_offeset($level)
/**
 * function for doing offset in a table
 *
 * @param   int     level
 * @return  string  offset
 */
function do_offset($level){
    $offset = "";
    for ($i=1; $i<$level;$i++){
        $offset = $offset . "<td id='tabledebug'></td>";
    }
    return $offset;
}

// }}}
// {{{ function show_array($array, $level, $sub)
/**
 * function for printing an array in a table
 *
 * @param   array   the array to be shown in a table
 * @param   int     number of levels
 * @param   int     sub
 * @return  void    return when finished
 */

function show_array($array, $level, $sub){
    if (is_array($array)){          // check if input is an array
       foreach($array as $key_val => $value) {
           $offset = "";
           if (is_array($value) == 1){   // array is multidimensional
               echo "<tr id='tabledebug'>";
               $offset = do_offset($level);
               echo $offset . "<td id='tabledebug'>" . $key_val . "</td>";
               show_array($value, $level+1, 1);
           } else {                        // (sub)array is not multidim
                if ($sub != 1){          // first entry for subarray
                    echo "<tr id='tabledebug'>";
                    $offset = do_offset($level);
                }
                $sub = 0;
                echo $offset . "<td id='tabledebug'>" . $key_val .
                "</td><td id='tabledebug'>" . $value . "</td>";
                echo "</tr>\n";
           }
       }
    } else { // argument $array is not an array
        return;
    }
}

// }}}
// {{{ function html_show_array($array, $title)
/**
 * function for showing a debug array in table, adding of divs
 *
 * @param   array   the array to create a html table from
 */

function html_show_array($array ){
    echo "<div id='tabledebug'><table>";
    show_array($array, 1, 0);
    echo "</div></table>";
}

// }}}
// {{{
function html_show_debug_info(){
    //echo "dennis" . $show_important = get_module_ini ('show_important');die;
    //if (!$show_important){
    //if (register::$vars['coscms_main']['show_creds'] == 0){
    /*
        unset(register::$vars['coscms_main']['url']);
        unset(register::$vars['coscms_main']['username']);
        unset(register::$vars['coscms_main']['password']);
        unset(register::$vars['coscms_main']['ssh_port']);
        unset(register::$vars['coscms_main']['ssh_host']);
        unset(register::$vars['coscms_main']['smtp_params_password']);
        unset(register::$vars['coscms_main']['development']['url']);
        unset(register::$vars['coscms_main']['development']['username']);
        unset(register::$vars['coscms_main']['development']['password']);
        unset(register::$vars['coscms_main']['development']['ssh_port']);
        unset(register::$vars['coscms_main']['development']['ssh_host']);
        unset(register::$vars['coscms_main']['development']['smtp_params_password']);
    //}*/

    if (register::$vars['coscms_main']['module']['debug'] == 1){
            html_show_array(array('sql_info' => db::getDebug()));
            html_show_array(register::$vars['coscms_debug']);
    }
}

/**
 * path class implements runlevel 5
 *
 * @package    google_analytics
 */
class debug {
    /**
     *
     * @param   int  $level the runlevel
     * @return  
     */
    public function runLevel($level){
        $str = '';
        if ($level == 6){
            //$ini = moduleLoader::getModuleIniSettings('debug');
            
            if (!get_module_ini('debug')){
                return;
            }
            
            // fetch all GLOBALS for debug
            register::$vars['coscms_debug']['_SESSION'] = &$_SESSION;
            register::$vars['coscms_debug']['_POST'] = &$_POST;
            register::$vars['coscms_debug']['_GET'] = &$_GET;
            register::$vars['coscms_debug']['_FILES'] = &$_FILES;
            register::$vars['coscms_debug']['_SERVER'] = &$_SERVER;
            
            // debug speed of application
            register::$vars['coscms_debug']['memory_usage'] = memory_get_peak_usage(false);

        }
        if (get_module_ini('debug')){
            ob_start();
            
            $_COS_BENCH = array();
            $_COS_BENCH['memory_usage'] = memory_get_peak_usage(false);
            $_COS_BENCH['timer']['start'] = register::$vars['coscms_debug']['timer']['start'];
            $_COS_BENCH['timer']['end'] = microtime(true);
            $_COS_BENCH['timer']['diff'] = $_COS_BENCH['timer']['end'] - $_COS_BENCH['timer']['start'] ;

            print "<a name=\"benchmark\">Benchmark</a>";
            html_show_array($_COS_BENCH);
            print "<a name=\"main_settings\">Main Settings</a>";
            if ($_SERVER['REMOTE_ADDR'] == get_module_ini('debug_allow_ip')){
                html_show_array(register::$vars['coscms_main']);
                print "<a name=\"module_settings\">Module Settings</a>";
                html_show_array(register::$vars['coscms_main']['module']);
            }
            
            print "<a name=\"sql\">SQL</a>";
            html_show_array(db::getDebug());

            print "<a name=\"server\">Server Info</a>";
            html_show_array($_SERVER);

            $str = ob_get_contents();
            ob_end_clean();
            template::setEndContent($str);
        }
    }
}
