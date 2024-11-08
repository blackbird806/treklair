module;

#include <SDL3/SDL.h>

export module treklair:sdlhelpers;

import :globals;

// I'm not sure if this is a good idea to implement this and if this is a good implementation with sdl
// I'll keep this on hold

// postfix sdl to avoid confusion with real sdl names
struct RendererStateSDL
{
	SDL_Texture* renderTarget;

	int rendererLogicalPresentationW, rendererLogicalPresentationH;
	SDL_RendererLogicalPresentation rendererLogicalPresentation;
	
	uint8_t drawColorR, drawColorG, drawColorB, drawColorA;
};

RendererStateSDL getCurrentRenderState()
{
	RendererStateSDL state;

	state.renderTarget = SDL_GetRenderTarget(sdl_renderer);
	SDL_GetRenderLogicalPresentation(sdl_renderer, &state.rendererLogicalPresentationW, &state.rendererLogicalPresentationH, &state.rendererLogicalPresentation);
	SDL_GetRenderDrawColor(sdl_renderer, &state.drawColorR, &state.drawColorG, &state.drawColorB, &state.drawColorA);

	return state;
}

void applyRenderState(RendererStateSDL const& state)
{
	SDL_SetRenderTarget(sdl_renderer, state.renderTarget);
	SDL_SetRenderLogicalPresentation(sdl_renderer, state.rendererLogicalPresentationW, state.rendererLogicalPresentationH, state.rendererLogicalPresentation);
	SDL_SetRenderDrawColor(sdl_renderer, state.drawColorR, state.drawColorG, state.drawColorB, state.drawColorA);
}

RendererStateSDL rendererStateStack[64];
int rendererStateStackIndex = 0;

void pushRenderState()
{
	rendererStateStack[rendererStateStackIndex++] = getCurrentRenderState();
}

void popRenderState()
{
	RendererStateSDL stateToApply = rendererStateStack[rendererStateStackIndex--];
	applyRenderState(stateToApply);
}