<?php

/**
 * model file for printing a debug array with usefull infomation
 *
 * @package    debug
 */

/**
 * function for doing offset in a table
 *
 * @param   int     level
 * @return  string  offset
 */
function debug_do_offset($level){
    $offset = "";
    for ($i=1; $i<$level;$i++){
        $offset = $offset . "<td id='tabledebug'></td>";
    }
    return $offset;
}

/**
 * function for printing an array in a table
 *
 * @param   array   the array to be shown in a table
 * @param   int     number of levels
 * @param   int     sub
 * @return  void    return when finished
 */

function debug_show_array($array, $level, $sub){
    if (is_array($array)){          // check if input is an array
       foreach($array as $key_val => $value) {
           $offset = "";
           if (is_array($value) == 1){   // array is multidimensional
               echo "<tr id='tabledebug'>";
               $offset = debug_do_offset($level);
               echo $offset . "<td id='tabledebug'>" . $key_val . "</td>";
               debug_show_array($value, $level+1, 1);
           } else {                        // (sub)array is not multidim
                if ($sub != 1){          // first entry for subarray
                    echo "<tr id='tabledebug'>";
                    $offset = debug_do_offset($level);
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

/**
 * function for showing a debug array in table, adding of divs
 *
 * @param   array   the array to create a html table from
 */

function debug_show_array_div($array ){
    echo "<div id=\"debug_table\">\n";
    echo "<table>\n";
    debug_show_array($array, 1, 0);
    echo "</table>\n";
    echo "</div>\n";
}

function debug_set_template_end(){
    
    if (config::$vars['coscms_main']['module']['debug'] == 1){
            ob_start();
            
            $_COS_BENCH = array();
            $_COS_BENCH['memory_usage'] = memory_get_peak_usage(false);
            $_COS_BENCH['timer']['start'] = config::$vars['coscms_debug']['timer']['start'];
            $_COS_BENCH['timer']['end'] = microtime(true);
            $_COS_BENCH['timer']['diff'] = $_COS_BENCH['timer']['end'] - $_COS_BENCH['timer']['start'] ;

            //echo "<a name=\"benchmark\">Benchmark</a>";
            debug_show_array_div($_COS_BENCH);
            //echo "<a name=\"main_settings\">Main Settings</a>";
            if ($_SERVER['REMOTE_ADDR'] == config::getModuleIni('debug_allow_ip')){
                debug_show_array_div(config::$vars['coscms_main']);
                echo "<a name=\"module_settings\">Module Settings</a>";
                debug_show_array_div(config::$vars['coscms_main']['module']);
            }
            
            //echo "<a name=\"sql\">SQL</a>";
            debug_show_array_div(db::getDebug());

            //echo "<a name=\"server\">Server Info</a>";
            debug_show_array_div($_SERVER);

            $str = ob_get_contents();
            ob_end_clean();
            ///echo $str;
            template::setEndContent($str);
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

            if (!config::getModuleIni('debug')){
                return;
            }

            
            // fetch all GLOBALS for debug
            config::$vars['coscms_debug']['_SESSION'] = &$_SESSION;
            config::$vars['coscms_debug']['_POST'] = &$_POST;
            config::$vars['coscms_debug']['_GET'] = &$_GET;
            config::$vars['coscms_debug']['_FILES'] = &$_FILES;
            config::$vars['coscms_debug']['_SERVER'] = &$_SERVER;
            
            // debug speed of application
            config::$vars['coscms_debug']['memory_usage'] = memory_get_peak_usage(false);

        }
        
        if (config::getModuleIni('debug')){
            ob_start();
            
            $_COS_BENCH = array();
            $_COS_BENCH['memory_usage'] = memory_get_peak_usage(false);
            $_COS_BENCH['timer']['start'] = config::$vars['coscms_debug']['timer']['start'];
            $_COS_BENCH['timer']['end'] = microtime(true);
            $_COS_BENCH['timer']['diff'] = $_COS_BENCH['timer']['end'] - $_COS_BENCH['timer']['start'] ;

            print "<a name=\"benchmark\">Benchmark</a>";
            debug_show_array_div($_COS_BENCH);
            print "<a name=\"main_settings\">Main Settings</a>";
            if ($_SERVER['REMOTE_ADDR'] == config::getModuleIni('debug_allow_ip')){
                debug_show_array_div(config::$vars['coscms_main']);
                print "<a name=\"module_settings\">Module Settings</a>";
                debug_show_array_div(config::$vars['coscms_main']['module']);
            }
            
            print "<a name=\"sql\">SQL</a>";
            debug_show_array_div(db::getDebug());

            print "<a name=\"server\">Server Info</a>";
            debug_show_array_div($_SERVER);

            $str = ob_get_contents();
            ob_end_clean();
            ///echo $str;
            template::setEndContent($str);
        }
    }
}

