
============================================================================

			glclock について

	２０００年６月
	Masa（川瀬 正樹）
	E-Mail: masa@daionet.gr.jp
	URL   : http://www.daionet.gr.jp/~masa

============================================================================

■■ 目次 ■■

● glclock とは
　・Windows 版関連ユーティリティ

● 動作環境
　・動作条件
　・glclock for Mac
　・推奨環境
　・環境固有条件

● インストール／アンインストール
　・アーカイブについて
　・インストール
　・アンインストール

● ベンチマーク「glclock Mark」について

● ソフト種別・著作権・免責事項
　・ソフト種別
　・著作権
　・免責事項

● その他
　・謝辞
　・参照ドキュメント
　・動作報告／ご意見・ご感想
　・最新版の入手方法

----------------------------------------------------------------------------

■■ glclock とは ■■

glclock は、OpenGL を利用した美しい懐中時計のプログラムです。
時計ですが、時計としての機能は最低限です（ただの懐中時計）。
どちらかといえばインタラクティブな３ＤＣＧデモンストレーションととらえて頂いた
方が良いでしょう。

マウスにより時計を操作できる他、フォグ、アンティエイリアシング、モーション
ブラー、被写界深度ブラー、ソリッド／環境テクスチャマッピング、半透明など、
さまざまな特殊処理を実行できます。

OpenGL 対応のビデオカードを使用すると、非常に快適に動作します。
なくても実行できますが、テクスチャマッピングやアルファブレンディングを行った際に
極端に速度が低下します。
是非、OpenGL をハードウェア実行できる環境でのご使用をお勧めします。

　● Windows 版関連ユーティリティ

　Windows 版では、関連ソフトウェアとして、glclock 専用ランチャソフト
　「glclock launcher」（フリーウェア）
　<http://www.daionet.gr.jp/~masa/glclock/launcher/>

　glclock をスクリーンセーバとして利用するための
　「glclock saver」（シェアウェア￥６００）
　<http://www.daionet.gr.jp/~masa/glclock/saver/>
　をリリースしています。

　これらのユーティリティを使うと、面倒なオプション指定をする必要もなく、
　さまざまな設定を容易に変更して試すことができます。
　Windows ユーザの方は、是非一度お試しください。

　※glclock-5.0 以前と glclock-5.0 よりも後のバージョンでは、これらのソフトは
  　一部互換性が保たれていません。
  　glclock-5.0 の時にダウンロードしていた方は、新しいバージョンを入手し直して
  　ください。


■■ 動作環境 ■■

　● 動作条件

　Intel 80x86 またはその互換系プロセッサ搭載の Windows 95/98/2000/NT、または、
　UNIX の X-Window 環境で動作します。
　OpenGL（Mesa 3D Graphics Library でも可）および GLUT ライブラリ
　<http://reality.sgi.com/opengl/glut3/glut3.html>
　が必要です。

　● glclock for Mac

　　Hirama 氏 <http://www.geocities.co.jp/SiliconValley/8916/> が
　　glclock を Macintosh 用に移植、コンパイルしてくださっています。
　　Macintosh ユーザは、是非お試しください。
　　詳細は glclock for Mac のウェブサイト
　　<http://www.geocities.co.jp/SiliconValley/8916/Macintosh/glclock.html>
　　をご参照ください。

　● 推奨環境

　　必須ではありませんが、以下のような条件を満たす環境であれば、より美しく、
　　快適に動作します。

　　▲ 推奨 OpenGL 能力（主に画質面）
　　　・テクスチャ・オブジェクトをサポートしていること
　　　　（OpenGL-1.1 以降もしくは GL_EXT_texture_object 拡張機能のサポート）
　　　・テクスチャをハードウェア処理可能で、256x256 以上のサイズを扱えること
　　　・バイリニア、ミップマップフィルタをサポートしていること
　　　・αグロー（α値に対するグローシェーディング）をサポートしていること
　　　・ブレンドファクタの GL_*_COLOR をサポートしていること

　　▲ 推奨 OpenGL 能力（主に速度面）
　　　・セパレート・スペキュラー・カラーをサポートしていること（OpenGL-1.2 以降
　　　　もしくは、GL_ARB_separate_specular_color, GL_EXT_separate_specular_color
　　    のいずれかのサポート）
　　　・マルチテクスチャをサポートしていること（GL_ARB_multitexture,
　　　　GL_EXT_multitexture, GL_SGIS_multitexture のいずれかのサポート）

　　▲ 快適動作の目安
　　　「Pentium2 233MHz + TNT」クラス以上の OpenGL 処理能力

　　▲ 超快適動作の目安
　　　「Pentium2 400MHz + TNT」クラス以上の OpenGL 処理能力

　● 環境固有条件

　　▲ Windows 環境

　　　OpenGL-1.1 以降に対応した opengl32.dll、glu32.dll が必要です。
　　　これらは Windows 98/2000/NT および Windows 95 OSR2 以降では標準です。

　　　これらの dll をお持ちでない方（初期の Windows 95）は、
　　　<ftp://ftp.microsoft.com/Softlib/MSLFILES/Opengl95.exe>	を入手し、
　　　含まれている *.dll を \WINDOWS\SYSTEM にコピーしてください。
　　　または、OpenGL を使用する他のソフトウェアに付属しているものを利用しても
　　　構いません。
　　　※Windows 版は、OpenGL バージョン 1.1 以降のランタイムが必須です。

　　　また、msvcrt.dll も必要です。この dll をお持ちでない方は、
　　　<http://www.daionet.gr.jp/~masa/archives/mfc42.lzh>
　　　をダウンロードし、指示にしたがってインストールしてください。
　　　これは、Bio_100% <http://bio.and.or.jp/> の
　　　metys 氏 <http://www.and.or.jp/~metys/> がランタイムをまとめて公開されて
　　　いるものです。
　　　再配布許可をいただきましたので、私のサイトにも置きました。

　　▲ Windows で Voodoo シリーズ搭載のビデオボードをお持ちの場合）

　　　○ Voodoo Graphics / Voodoo2 / Voodoo Rush

　　　　3Dfx OpenGL Beta 2.1 または Mesa Voodoo driver を利用します。
　　　　ただし、以下のものが必要です。

　　　　・Voodoo、Voodoo2 または Voodoo Rush を搭載したビデオカード
　　　　・Glide 2.31 または Glide 2.4x のランタイム（glide2x.dll）

　　　　これらは 3Dfx 社のサイト <http://www.3dfx.com/> から適宜入手してください。

　　　　また、3Dfx OpenGL Beta 2.1 もしくは Mesa Voodoo のスタンドアローン
　　　　ドライバ（opengl32.dll）が必要です。
　　　　<http://www.daionet.gr.jp/~masa/archives/3DfxOpenGL32DLL.zip>
　　　　<http://www.daionet.gr.jp/~masa/archives/MesaGlideOpenGL32DLL.zip>
　　　　のいずれかをダウンロードし、含まれている opengl32.dll を glclock.exe
　　　　のあるフォルダにコピーしてください。
　　　　※Windows のシステムフォルダにコピーしないよう特に注意してください。

　　　　makeshort.exe を実行し、「Window Size」で、

　　　　・640 x 480 (e.g. 3Dfx Voodoo Graphics)
　　　　・800 x 600 (e.g. Voodoo2, Voodoo Rush)
　　　　・1024 x 768 (e.g. Voodoo2 SLI)

　　　　のいずれかを選択します。
　　　　それぞれの違いはディスプレイ解像度だけですので、カードに適したモードを
　　　　選択してください（Mesa Voodoo driver では 800 x 600 が最高のようです）。

　　　　Quake III Compatible Driver でも一応動作しますが、不安定ですのでお勧め
　　　　できません。
　　　　逆に最近の Mesa Voodoo driver はかなり出来が良いようです。
　　　　テクスチャ使用時のフォグやトライリニア・フィルタリングが使用可能など
　　　　3Dfx OpenGL Beta 2.1 より優れている部分もありますので、興味のある方は
　　　　お試しください。

　　　○ Voodoo3 / Voodoo Banshee（Rush も可？）

　　　　3Dfx 社 <http://www.3dfx.com/> が提供している
　　　　3Dfx OpenGL ICD (Quake III Compatible Driver) が必要です。

　　　　Quake III Compatible は、Quake III での動作しか確認されていません。
　　　　一般的な OpenGL アプリケーションでは不具合も報告されているようですので、
　　　　インストールされる方は予めご承知置きください。

　　▲ X-Window 環境

　　　各環境でコンパイルする必要があります。
　　　C++ コンパイラおよび、Xlib、OpenGL または Mesa 3D Graphics Library、
　　　GLUT ライブラリなどが必要です。

　　　GLUT ライブラリは、バージョン 3.7 Beta 以降をお勧めします。
　　　必須ではありませんが、キーボード操作がより自然になり、また、
　　　フルスクリーン時の解像度、色数等の指定が可能になります。

　　　Mesa Library は <http://www.mesa3d.org/> から、
　　　GLUT は <http://reality.sgi.com/opengl/glut3/glut3.html> から
　　　それぞれ入手できます。


■■ インストール／アンインストール ■■

　● アーカイブについて

　　glclock のアーカイブには、以下のようなファイルが含まれています。

　　▲ 共通ファイル

　　　README.txt	このファイル
　　　GUIDE.txt		実行ガイド、お勧めオプション
　　　MANUAL.txt	操作方法、オプションについての詳細
　　　HISTORY.txt	glclock 更新履歴
　　　GLCLOCKMARK.txt	ベンチマークソフト「glclock Mark」についての説明
　　　TROUBLESHOOT.txt	環境固有の問題点および解決方法など
　　　*.ppm / *.pgm	テクスチャマッッピング用のサンプル画像
　　　*.log		glclock Mark の参照環境の結果
　　　glclockmark_ALL	すべての解像度でベンチマークを実行するスクリプト

　　▲ Windows 環境

　　　glclock.exe	実行ファイル
　　　glclockmark.exe	ベンチマーク（glclock Mark）実行ファイル
　　　makeshort.exe	さまざまな時計用ショートカットの自動作成プログラム
　　　glut32.dll	GLUT ランタイムライブラリ

　　▲ X-Window 環境用

　　　*clock*		便利な起動コマンド（sh スクリプト）
　　　COMPILE.txt	コンパイル方法についての説明
　　　makefile*		コンパイル用サンプルメイクファイル
　　　*.cpp / *.H	glclock ソースプログラム（C++ 言語）

　● インストール

　　アーカイブを適当なディレクトリに展開します。

　　▲ Windows 版

　　　makeshort.exe を実行します。
　　　「Window Size」で、ウィンドウモードもしくはフルスクリーンの好みのサイズを
　　　選択し、「OK」をクリックしてください。
　　　glclock のフォルダにさまざまなオプション付きのショートカットが作成されます。

　　　Voodoo Graphics / Voodoo2 のグラフィックスカードをお使いの場合は、必ず

　　　・640 x 480 (e.g. 3Dfx Voodoo Graphics)
　　　・800 x 600 (e.g. Voodoo2, Voodoo Rush)
　　　・1024 x 768 (e.g. Voodoo2 SLI)

　　　のいずれか（フルスクリーン）を選択してください（Mesa Voodoo driver では
　　　800 x 600 が最高のようです）。

　　　「Enable Detail-Texture if available」（デフォルトでチェック）は、
　　　可能であればディティールテクスチャを有効にし、よりきめ細かい表現を行います。

　　　「Show FPS in Window」をチェックすると、フレームレート
　　　（FPS: Frames Per Second）を実行ウィンドウおよびログに出力します。

　　　「Bug Fix options」は、環境固有の問題を解消するためのオプションです。
　　　実行環境によって自動的にセットされますので、通常ユーザが気にする必要は
　　　ありません。

　　　「Common options」を直接編集し、任意のオプションを追加することができます。
　　　表示されているオプションは、ショートカットに共通に追加されます。
　　　コマンドラインオプションについては、MANUAL.txt をご覧ください。

　　　※ショートカットを初期状態に戻したい場合や、glclock をフォルダごと移動した
　　　　場合は、makeshort.exe を再度実行してください。

　　▲ UNIX X-Window

　　 コンパイルが必要です。
　　 COMPILE.TXT を参照し、各環境でコンパイルしてください。

　　▲ 環境変数について（主に UNIX X-Window 環境）

　　　glclock は、テクスチャ使用時の画像ファイルを、

　　　　1. "-ip" オプションで指定したディレクトリ
　　　　2. glclock を実行したカレントディレクトリ
　　　　3. 環境変数 "GLCLOCK_IMAGE_PATH" に指定されているディレクトリ

　　　の順に検索します。
　　　いずれにも該当するファイルが見つからなかった場合はエラーとなり、
　　　テスト用のテクスチャが生成されて使用されます。

　　　UNIX 環境では、環境変数を設定しておくことをお薦めします。
　　　例えば csh シェルをお使いなら、ホームディレクトリの .cshrc に、
　　　setenv GLCLOCK_IMAGE_PATH $HOME/images
　　　のような一行を加えます。
　　　$HOME/images のところに、画像ファイルをまとめて置いてあるディレクトリを
　　　書いておくと良いでしょう。

　　　※ディレクトリは１つしか指定できません。
　　　　そのうちいくつでも指定可能なフォーマットにする...かもしれません。

　● アンインストール

　　glclock をディレクトリごと削除してください。

　　Windows 版では、システムフォルダ（通常 C:\WINDOWS\SYSTEM）に、
　　glclock.ini という glclock.exe の絶対パスを記述したファイルが生成されて
　　います。
　　ゴミが残るのが嫌な方は、このファイルも削除してください。

　　環境変数等をセットした方は、適宜削除してください。


■■ ベンチマーク「glclock Mark」について ■■

glclock 専用の OpenGL ベンチマークソフトです。

OpenGL のさまざまな能力（ライティング、テクスチャマッピング、
環境マッピング、ブレンディング、ポリゴンスループット etc.）を測定します。
結果はテキストログだけでなく、HTML でグラフ化されますので、
実行環境の OpenGL の速度や特性（得手／不得手等）が一目で判ります。

スコアは、PentiumII-233MHz + RivaTNT PCI-16MB 320 x 320 x 16bpp
に対する速度比（パーセンテージ表記）になります。
glclock Mark についての詳細は、GLCLOCKMARK.txt を参照してください。


■■ ソフト種別・著作権・免責事項 ■■

　● ソフト種別

　　glclock はフリーソフトウェアです。

　　二次配布、改変などについては、自由に行なって頂いて構いません。
　　また、改変を伴う配布についても、改変者名および改変内容を明記していただければ、
　　自由に行って頂いて結構です。

　　雑誌等への掲載時は、掲載誌を一部頂けると幸いです。
　　電子メール等でご連絡頂ければ、送付の際の宛先をお知らせ致します。

　● 著作権

　　オリジナルのアーカイブに含まれているプログラムの著作権は
　　[ Masa こと 川瀬 正樹 <masa@daionet.gr.jp> ] が保有します。

　● 免責事項

　　本ソフトによって発生したいかなる障害に対しても、著作者は一切の責任を負わない
　　ものといたします。予め御了承ください。
　　滅多なことはないはずですが、実行はあくまで各自の責任で行ってください。


■■ その他 ■■

　● 謝辞

　　▲ 本ソフトウェアを開発するににあたり、ppm 画像ローダおよびサンプル画像の
　　　一部を提供してくれた Nacha こと 川瀬直樹（さん^^;）に感謝します。

　　▲ Macintosh 用に移植し、glclock for Mac を公開してしてくださっている
　　　Hirama 氏 <http://www.geocities.co.jp/SiliconValley/8916/> に
　　　心から感謝いたします。

　　▲ 重要なバグリポートをくださった nVIDIA OpenGL group の Nick Triantos 氏に
　　　心から感謝いたします。
　　　Thank you so very much Nick!

　● 参照ドキュメント

　　▲ X-Window 環境でのコンパイル方法については、COMPILE.txt をご参照ください。
　　▲ 実行方法・使い方（特に初めて使われる場合）は、GUIDE.txt をご参照ください。
　　▲ 操作方法一覧・各オプションについては、MANUAL.txt をご参照ください。
　　▲ ベンチマーク「glclock Mark」については、GLCLOCKMARK.txt をご参照ください。
　　▲ 環境依存・既知の問題点については、TROUBLESHOOT.txt をご参照ください。

　● 動作報告／ご意見・ご感想

　　みなさんの御意見・御感想をお待ちしています。<E-Mail: masa@daionet.gr.jp>

　　また、X-Window 環境でコンパイル・実行に何らかの問題が発生した場合は
　　リポートを頂けると、今後の版で修正する際に参考になります。
　　ただし、不具合の修正・損害保証義務は負いませんので、予めご了承ください。

　● 最新版の入手方法

　　本ソフトの最新版は、常に（多分）作者の WEB サイト
　　<http://www.daionet.gr.jp/~masa> よりダウンロードできます。
