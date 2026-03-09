import socket
import sys
# Pico側の設定に合わせたIPアドレスとポート番号
TARGET_IP = "192.168.40.50"
TARGET_PORT = 5005
def main():
    # UDP通信用のソケットを作成 (IPv4, UDP)
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    except Exception as e:
        print(f"ソケットの作成に失敗しました: {e}")
        sys.exit(1)
    # 起動時のUI表示
    print("=========================================")
    print("       Pico UDP Control Test Tool        ")
    print("=========================================")
    print(f" Target : {TARGET_IP}:{TARGET_PORT}")
    print("-----------------------------------------")
    print(" [Commands]")
    print("  - ALLON   : 全リレー(GP0-GP7)をON")
    print("  - ALLOFF  : 全リレー(GP0-GP7)をOFF")
    print("  - CHx_ON  : 個別ON  (例: CH0_ON, CH7_ON)")
    print("  - CHx_OFF : 個別OFF (例: CH2_OFF)")
    print("  - exit    : テスト終了")
    print("=========================================")
    try:
        while True:
            # ターミナルからの入力待ち
            cmd = input("Send Command > ").strip()
            # 終了コマンドの処理
            if cmd.lower() in ['exit', 'quit']:
                print("テストを終了します。")
                break
            # 空エンターの場合はスキップ
            if not cmd:
                continue
            # 入力された文字列をUTF-8のバイト列に変換して送信
            try:
                sock.sendto(cmd.encode('utf-8'), (TARGET_IP, TARGET_PORT))
                print(f" -> [Sent] {cmd}")
            except Exception as e:
                print(f" -> [Error] 送信失敗: {e}")
    except KeyboardInterrupt:
        # Ctrl+Cで強制終了された場合の処理
        print("\nテストを強制終了します。")
    finally:
        # プログラム終了時に必ずソケットを閉じる
        sock.close()
if __name__ == "__main__":
    main()