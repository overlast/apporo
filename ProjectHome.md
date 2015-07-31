# Apporo #
Apporo is one of the Approximate String Matching Engine.

In example, it can use for spell correction for search query of a medium scale web service.

## How to Install ##
### Mac OS X and Linux ###
When you install Apporo, you have to install at least one of the following suffix array libraries.

- tsubomi(If you already installed tsubomi, you have to do "svn update;make;sudo make install" to install [r55](http://code.google.com/p/tsubomi/source/detail?r=55) or later.)<br>
-- <a href='http://code.google.com/p/tsubomi/'>http://code.google.com/p/tsubomi/</a><br>
<pre><code>svn checkout http://tsubomi.googlecode.com/svn/trunk/ tsubomi<br>
cd tsubomi/src/tsubomi<br>
make<br>
sudo make install<br>
</code></pre>

After you installed suffix array library, you can compile Apporo by following way.<br>

<pre><code>svn checkout http://apporo.googlecode.com/svn/trunk/ apporo<br>
cd apporo/src<br>
make<br>
sudo make install<br>
</code></pre>

After installing, you can index and search your TSV file.<br>
<br>
<h2>How to Uninstall</h2>
<h3>Mac OS X and Linux</h3>
<pre><code>cd apporo/src<br>
sudo make uninstall<br>
</code></pre>

<h2>How to Index</h2>
When you use Apporo, you have to prepare a TSV file.<br>
<br>
Apporo will index the first column of  your TSV file.<br>
Before start to index, you should sort the TSV file by the first column of the TSV file.<br>

If your TSV file includes many multi byte characters, you should use the index of UTF-8 char mode.<br>
<br>
You can index your TSV file by following way.<br>
<br>
- UTF-8 char mode example<br>
<pre><code>% LC_ALL=C sort -t $'\t' -k 1,1 [your TSV file] &gt; [sorted TSV file]<br>
% apporo_indexer -i [sorted TSV file] -u -bt <br>
% apporo_indexer -i [sorted TSV file] -d<br>
</code></pre>

- ASCII mode example<br>
<pre><code>% LC_ALL=C sort -t $'\t' -k 1,1 [your TSV file] &gt; [sorted TSV file]<br>
% apporo_indexer -i [your TSV file] -bt<br>
% apporo_indexer -i [your TSV file] -d<br>
</code></pre>

<h2>How to Search</h2>
<h3>with command line options</h3>
After finishing to index, you can search the first column of your TSV file from the index by following way.<br>
<br>
If index mode is UTF-8, you should set the "-u" parameter to use UTF-8 char mode.<br>
<br>
- UTF-8 char mode example<br>
<pre><code>% apporo_searcher -i [your TSV file] -u -s -p -r 10 -t [string distance threshold(0.0 to 1.0)] -q [your query]<br>
</code></pre>

- ASCII mode example<br>
<pre><code>% apporo_searcher -i [your TSV file] -s -p -r 10 -t [string distance threshold(0.0 to 1.0)] -q [your query]<br>
</code></pre>

<h3>with configuration file</h3>
You can also search your TSV file from the index using a configuration file for Apporo.<br>
You can see a example of the configuration file.<br>
<br>
<pre><code>$ cd apporo/config<br>
$ cat ./sample.conf<br>
ngram_length	2<br>
is_pre	true<br>
is_suf	true<br>
is_utf8	false<br>
dist_threshold	0.6<br>
index_path	path to your file which already indexed.<br>
dist_func  cosine<br>
entry_buf_len	1024<br>
engine	tsubomi<br>
result_num	10<br>
bucket_size	2000<br>
is_surface	true<br>
is_kana	false<br>
is_roman	false<br>
is_mecab	false<br>
is_juman	false<br>
is_kytea	false<br>
</code></pre>

This sample file is including all options for search process.<br>
The format of configuration file of Apporo is TSV file.<br>
<br>
You must set the file path of your TSV as index_path in configuration file.<br>
<br>
And you can search using your configuration file by following way.<br>
<br>
<pre><code>% apporo [path to your configuration file] [search query]<br>
</code></pre>

<h3>Search Options</h3>
If index mode is UTF-8, you should set the "is_utf8" or "-u" parameter to use UTF-8 char mode.<br>
<br>
<table><thead><th>parameter name</th><th>command line option</th><th>value type</th><th>meaning</th><th>value area</th></thead><tbody>
<tr><td>ngram_length  </td><td>-n N               </td><td>int       </td><td>ngram width which is used to split search query</td><td>N >= 1    </td></tr>
<tr><td>is_pre        </td><td>-p                 </td><td>boolean   </td><td>generate the ngrams which include null character of left of head character of entry</td><td>true/false</td></tr>
<tr><td>is_suf        </td><td>-s                 </td><td>boolean   </td><td>generate the ngrams which include null character of right of tail character of entry</td><td>true/false</td></tr>
<tr><td>is_utf8       </td><td>-u                 </td><td>boolean   </td><td>ASCII index mode or UTF-8 index mode</td><td>true(UTF-8)/false(ASCII)</td></tr>
<tr><td>dist_threshold</td><td>-t N               </td><td>double    </td><td>Threshold of the search result score</td><td>0.0 <= N <= 1.0</td></tr>
<tr><td>index_path    </td><td>-i S               </td><td>string    </td><td>File path of original file which you indexed</td><td>absolute path to your file</td></tr>
<tr><td>dist_func     </td><td>-d S               </td><td>string    </td><td>string distance to pruning search process</td><td>cosine, dice, jaccard or overlap</td></tr>
<tr><td>entry_buf_len </td><td>-b N               </td><td>int       </td><td>data size of buffer which is used for temporary data area for each result entry by searcher</td><td>N > 1 (default = 1024, which must extend to fit for biggest entry of your TSV file)</td></tr>
<tr><td>engine        </td><td>-e S               </td><td>string    </td><td>storage engine for index</td><td>tsubomi   </td></tr>
<tr><td>result_num    </td><td>-r N               </td><td>int       </td><td>how many entries do want to get as result by search</td><td>N >= 0    </td></tr>
<tr><td>bucket_size   </td><td>-B N               </td><td>int       </td><td>temporary data bucket number to rerank the result list</td><td>N >=1 (I recommend more than  result_num times 200 )</td></tr>
<tr><td>is_surface    </td><td>none               </td><td>boolean   </td><td>activate surface search mode</td><td>true/false</td></tr>
<tr><td>is_kana       </td><td>none               </td><td>boolean   </td><td>activate kana character search mode(not implemented)</td><td>true/false</td></tr>
<tr><td>is_roman      </td><td>none               </td><td>boolean   </td><td>activate roman character search mode(mot implemented)</td><td>true/false</td></tr>
<tr><td>is_mecab      </td><td>none               </td><td>boolean   </td><td>using MeCab to get readings(not implemented)</td><td>true/false</td></tr>
<tr><td>is_juman      </td><td>none               </td><td>boolean   </td><td>using JUMAN to get readings(not implemented)</td><td>true/false</td></tr>
<tr><td>is_kytea      </td><td>none               </td><td>boolean   </td><td>using KyTea to get readings(not implemented)</td><td>true/false</td></tr></tbody></table>

<h2>Language binding</h2>
I already implemented the Perl binding.<br>
<br>
<h3>for Perl</h3>
You can install the Perl binding for Apporo by following way.<br>
<br>
<pre><code>% cd apporo/perl/Apporo<br>
% perl Makefile.pl<br>
% make<br>
% sudo make install<br>
</code></pre>

And this is usage of Perl binding for Apporo.<br>
<br>
- sample.pl<br>
<pre><code>#!/usr/bin/env perl<br>
<br>
use strict;<br>
use warnings;<br>
use utf8;<br>
use YAML;<br>
<br>
use Apporo;<br>
<br>
my $config_path = $ARGV[0];<br>
my $query = $ARGV[1];<br>
my $app = Apporo-&gt;new($config_path);<br>
my @arr = $app-&gt;retrieve($query);<br>
print Dump \@arr;<br>
</code></pre>

After indexing and writing your configuration file,  you can search your TSV file by following way.<br>
<br>
<pre><code>% ./sample.pl [path of your configuration file] [search query]<br>
</code></pre>

<h3>for Python 2.x</h3>
You can install the Python binding for Apporo by following way.<br>
<br>
<pre><code>% cd apporo/python/python_2.x/ <br>
% sudo python ./setup.py install<br>
</code></pre>

And this is usage of Python binding for Apporo.<br>
<br>
- apporo/python/python_2.x/t/test_class_import.py<br>
<pre><code>#!/usr/bin/env python<br>
#-*- coding:utf-8 -*-<br>
import sys<br>
from apporo_python import Apporo<br>
<br>
app = Apporo(sys.argv[1]);<br>
ret = app.retrieve(sys.argv[2])<br>
for item in ret:<br>
    print item<br>
app.delete()<br>
</code></pre>

After indexing and writing your configuration file,  you can search your TSV file by following way.<br>
<br>
<pre><code>% apporo/python/python_2.x/t/test_class_import.py [path of your configuration file] [search query]<br>
</code></pre>

There is another example to imoprt Apporo in "python/python_2.x/t".<br>
<br>
<h3>for Python 3.x, Ruby and Java</h3>
I will implement the bindings for these languages.<br>
<br>
<h2>ToDO</h2>
<h3>annoying but important</h3>
<ul><li>Writing API document and comment of source code<br>
</li><li>Writing many tests<br>
<h3>very easy</h3>
</li><li>Implementing normalize process for index and query<br>
</li><li>Implementing  kana character search (for Japanese)<br>
</li><li>Implementing roman character search (for Japanese)<br>
</li><li>Implementing index splitting with character numbers of each entry<br>
</li><li>Implementing Language Binding for Python, Ruby and Java<br>
</li><li>Uploading the Perl binding to CPAN<br>
<h3>easy</h3>
</li><li>Optimizing<br>
<h3>a little difficult</h3>
</li><li>Adapting other data structures<br>
</li><li>Implementing my data store libraries<br>
</li><li>etc.</li></ul>

<h2>Acknowledgment</h2>
<ul><li>@echizen_tm<br>
</li><li>@machy<br>
</li><li>@shnya_m<br>
</li><li>@uchumik<br>
</li><li>@yto</li></ul>

<h2>Other Information</h2>
Apporo is written by Toshinori Sato.<br>
You can contact me by Twitter to @overlast.