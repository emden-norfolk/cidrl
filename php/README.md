# cidrl.php
List All IP Addresses within a CIDR Block (PHP)

## Synopsis

```php
require_once 'cidrl.php';
var_dump(cidrl('194.168.0.1/28'));
```

Results in:

```
array(16) {
  [0]=>
  string(11) "194.168.0.0"
  [1]=>
  string(11) "194.168.0.1"
  [2]=>
  string(11) "194.168.0.2"
  [3]=>
  string(11) "194.168.0.3"
  [4]=>
  string(11) "194.168.0.4"
  [5]=>
  string(11) "194.168.0.5"
  [6]=>
  string(11) "194.168.0.6"
  [7]=>
  string(11) "194.168.0.7"
  [8]=>
  string(11) "194.168.0.8"
  [9]=>
  string(11) "194.168.0.9"
  [10]=>
  string(12) "194.168.0.10"
  [11]=>
  string(12) "194.168.0.11"
  [12]=>
  string(12) "194.168.0.12"
  [13]=>
  string(12) "194.168.0.13"
  [14]=>
  string(12) "194.168.0.14"
  [15]=>
  string(12) "194.168.0.15"
}
```
