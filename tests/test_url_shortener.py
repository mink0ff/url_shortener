# Start the tests via `make test-debug` or `make test-release`

import pytest

from testsuite.databases import pgsql  # noqa: F401

@pytest.mark.asyncio
async def test_create_short_url(service_client):
    """Создание короткого URL"""
    response = await service_client.post(
        "/create",
        json={"original_url": "https://example.com"}
    )

    assert response.status == 201 
    data = response.json()
    assert "short_url" in data
    assert isinstance(data["short_url"], str)


@pytest.mark.asyncio
async def test_redirect_with_forwarding_prohibited(service_client):
    """Редирект на короткий URL с запрещенной автоматической переадресацией"""
    create_resp = await service_client.post(
        "/create", json={"original_url": "https://example.com"}
    )
    short_url = create_resp.json()["short_url"]

    redirect_resp = await service_client.get(f"/redirect/{short_url}", allow_redirects=False)
    assert redirect_resp.status == 302
    assert redirect_resp.headers["Location"] == "https://example.com"


@pytest.mark.asyncio
async def test_redirect_with_forwarding_permission(service_client):
    """Редирект на короткий URL с разрешенной автоматической переадресацией"""
    create_resp = await service_client.post(
        "/create", json={"original_url": "https://example.com"}
    )
    short_url = create_resp.json()["short_url"]

    redirect_resp = await service_client.get(f"/redirect/{short_url}")
    assert redirect_resp.status == 200


@pytest.mark.asyncio
async def test_create_invalid_url(service_client):
    """Создание короткого URL с некорректным URL"""
    resp = await service_client.post("/create", json={"original_url": "not_a_url"})
    assert resp.status == 400
    data = resp.json()
    assert "error" in data
    assert data["error"] == "Invalid URL format"

@pytest.mark.asyncio
async def test_redirect_not_found(service_client):
    """Редирект на несуществующий короткий URL"""
    resp = await service_client.get("/redirect/nonexistent123", allow_redirects=False)
    assert resp.status == 404
    data = resp.json()
    assert "error" in data
    assert data["error"] == "Short URL not found"


@pytest.mark.asyncio
async def test_delete_short_url(service_client):
    """Удаление существующего короткого URL"""
    create_resp = await service_client.post(
    "/create", json={"original_url": "https://example.com"}
    )
    short_url = create_resp.json()["short_url"]

    delete_resp = await service_client.delete(f"/delete", json={"short_url": f"{short_url}"})
    assert delete_resp.status == 200
    data = delete_resp.json()
    assert data.get("deleted_short_url") == f"{short_url}"

    redirect_resp = await service_client.get(f"/redirect/{short_url}", allow_redirects=False)
    assert redirect_resp.status == 404


@pytest.mark.asyncio
async def test_delete_short_url_not_found(service_client):
    """Попытка удалить несуществующий короткий URL"""
    delete_resp = await service_client.delete("/delete", json={"short_url": "nonexistent123"})
    assert delete_resp.status == 404
    data = delete_resp.json()
    assert "error" in data
    assert data["error"] == "Short URL not found for deletion"

