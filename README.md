# blog-relay

技術ブログ「LAN対応24ch入力×8chリレー制御装置を作る | アイゼック株式会社」の関連ソースをまとめるリポジトリです。

関連記事:

- 第1回: https://isecj.jp/blog/raspberry-pi-pico-lan-relay-01/
- 第2回: https://isecj.jp/blog/raspberry-pi-pico-lan-relay-02/

## このリポジトリに含まれるもの

### `raspberry-pi-pico-lan-relay-02`

第2回の記事で使用したコード一式です。

- `raspberry-pi-pico-lan-relay-02.cpp`
  Raspberry Pi Pico と ENC28J60 を接続し、UDP コマンドで GPIO を制御する確認用プログラムです。
- `raspberry-pi-pico-lan-relay-02.py`
  PC 側から UDP コマンドを送信して動作確認するためのテスト用スクリプトです。

### `ProPrj_Lan_Relay_blog_2026-03-05_10-33-51_2026-03-09.epro`

EasyEDA の設計ファイルです。

現時点では第2回の記事で使用した図面のみを含んでいます。
今後、第3回の記事を執筆する際に EasyEDA で図面を追加し、この `.epro` ファイルも更新して第3回の記事用の図面を追記していく予定です。

## リポジトリ運用方針

- 第2回の記事に対応するコードは `raspberry-pi-pico-lan-relay-02` に保存します。
- 第3回以降の記事で使用するコードや図面も、このリポジトリに順次追加していきます。
- EasyEDA の設計ファイルは記事の進行に合わせて更新し、ブログ本文から参照できるようにします。
