# Unreal Engine 5 プロジェクト開発ガイドライン

このプロジェクトは Unreal Engine 5.8 の C++ プロジェクト（MyProjectTest）です。

## 基本方針
- C++ ソースコードは `Source/MyProjectTest/` 配下に配置されています。
- Unreal Engine のコーディング規約（パスカルケース、プレフィックス `A` (Actor), `U` (Object/Component), `F` (Struct), `E` (Enum) 等）を遵守します。
- UE5 の MCP (Model Context Protocol) サーバーが利用可能な場合は、UE5 側のツールを活用してアセットやエディタの操作を行います。
