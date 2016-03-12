<?php

namespace modules\debug;

use diversen\conf;
use diversen\db\admin;
use diversen\db\connect;
use diversen\db\q;
use diversen\file\string as fileStr;
use diversen\template;
use diversen\profile;

/**
 * Debug module class
 */
class module {
    
    /**
     * Checks if connection is MySQL
     * @return boolean $res
     */
    public function isMySQL () {
        $info = admin::getDbInfo();
        if ($info['scheme'] == 'mysql' OR $info['scheme'] == 'mysqli') {
            return true;
        }
        return false;
    }

    /**
     * Echo HTML debug tables
     * @param int  $level the runlevel
     * @return void
     */
    public function runLevel($level) {

        // Only on level 1
        if ($level == 1) {
            if ($this->isMySQL()) {
                q::query("set profiling=1;")->exec();
            }
            return;
        }

        // Level 7
        $debug = conf::getMainIni('debug');
        if (!$debug) {
            return '';
        }

        ob_start();
        
        echo "<br />";
        echo '<a name="main-settings" class="main-settings">All ini Settings</a>';
        
        $p = new profile();
        $config = $p->iniArrayPrepare(conf::$vars['coscms_main']);
        self::echoArrayDiv($config, 'debug-main');
                
        echo '<a name="modules" class="modules">Only Module Settings</a>';
        self::echoArrayDiv(conf::$vars['coscms_main']['module'], 'debug-modules');

        echo '<a name="sql" class="sql">SQL</a>';
        self::echoArrayDiv(connect::getDebug(), 'debug-sql');
        
        if ($this->isMySQL()) {
            $rows = q::query('SHOW PROFILES;')->fetch();            
            echo '<a name="profiler" class="profiler">MySQL profiler</a>';
            self::echoArrayDiv($rows, 'debug-profiler');
        }
      
        echo '<a name="server" class="server">Server Info</a>';
        self::echoArrayDiv($_SERVER, 'debug-server');

        echo '<a name="get" class="get">$_GET</a>';
        self::echoArrayDiv($_GET, 'debug-get');
        
        echo '<a name="post" class="post">$_POST</a>';
        self::echoArrayDiv($_POST, 'debug-post');
        
        echo '<a name="cookie" class="cookie">$_COOKIE</a>';
        self::echoArrayDiv($_COOKIE, 'debug-cookie');
        
        $error_file = conf::pathBase() . "/logs/error.log";
        if (file_exists($error_file)) {
            $log = array_reverse(fileStr::getTail($error_file, 10));
        }
        echo '<a name="log" class="log">Error log</a>';
        self::echoArrayDiv($log, 'debug-log');
        
        ?>
<script>

$( ".main-settings" ).click(function() {
  $( ".debug-main" ).toggle();
});

$( ".modules" ).click(function() {
  $( ".debug-modules" ).toggle( );
});

$( ".sql" ).click(function() {
  $( ".debug-sql" ).toggle();
});

$( ".profiler" ).click(function() {
  $( ".debug-profiler" ).toggle();
});

$( ".server" ).click(function() {
  $( ".debug-server" ).toggle( );
});

$( ".get" ).click(function() {
  $( ".debug-get" ).toggle( );
});

$( ".post" ).click(function() {
  $( ".debug-post" ).toggle( );
});

$( ".cookie" ).click(function() {
  $( ".debug-cookie" ).toggle( );
});

$( ".log" ).click(function() {
  $( ".debug-log" ).toggle( );
});

</script><?php

        $str = ob_get_contents();
        ob_end_clean();
        template::setEndContent($str);
    }

    /**
     * function for doing offset in a table
     *
     * @param   int     level
     * @return  string  offset
     */
    public static function getOffset($level) {
        $offset = "";
        for ($i = 1; $i < $level; $i++) {
            $offset = $offset . "<td></td>";
        }
        return $offset;
    }

    /**
     * Method that echos an array as a HTML table
     * @param   array   the array to be shown in a table
     * @param   int     number of levels
     * @param   int     sub
     * @return  void    return when finished
     */
    public static function displayArray($array, $level, $sub) {
        if (is_array($array)) {          // check if input is an array
            foreach ($array as $key_val => $value) {
                $offset = "";
                if (is_array($value) == 1) {   // array is multidimensional
                    echo "<tr>";
                    $offset = self::getOffset($level);
                    echo $offset . "<td>" . $key_val . "</td>";
                    self::displayArray($value, $level + 1, 1);
                } else {
                    if ($sub != 1) {
                        echo "<tr>";
                        $offset = self::getOffset($level);
                    }
                    $sub = 0;
                    
                    echo $offset . 
                            "<td>" . 
                            $key_val .
                            "</td>" . 
                            "<td>" . 
                            $value . 
                            "</td>";
                    
                    echo "</tr>\n";
                }
            }
        } else { 
            return;
        }
    }

    /**
     * function for showing a debug array in table, adding of divs
     *
     * @param   array   the array to create a html table from
     */
    public static function echoArrayDiv($array, $class) {

        echo '<div class="debug-table ' . $class . '" style="display: none">';
        echo '<table class="uk-table">';
        self::displayArray($array, 1, 0);
        echo "</table>\n";
        echo "</div>\n";
        echo "<br />";
    }
}
